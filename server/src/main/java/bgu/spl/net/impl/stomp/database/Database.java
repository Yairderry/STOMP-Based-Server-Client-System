package bgu.spl.net.impl.stomp.database;

import java.util.*;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class Database {

    // key: channel-name , value: list of users subscribed to the channel
    private final HashMap<String, Set<User>> channels = new HashMap<>();
    private final HashMap<String, ReadWriteLock> channelsLocks = new HashMap<>();
    // key: username , value: user
    private final HashMap<String, User> users = new HashMap<>();

    public Set<User> getChannel(String channel) {
        return channels.getOrDefault(channel, null);
    }

    public String trySubscribe(String subscriptionId, String channel, User user){
        try{
            user.lock(true);

            channels.putIfAbsent(channel, new HashSet<User>());
            channelsLocks.putIfAbsent(channel, new ReentrantReadWriteLock());
            
            lock(channel, true);
            Set<User> channelUsers = channels.get(channel);
            if (channelUsers.contains(user))
                return "User already subscribed to this topic";
            channelUsers.add(user);
            user.addSubscription(subscriptionId, channel);
        }
        finally{
            unlock(channel, true);
            user.unlock(true);
        }
        return "";
    }

    public String tryUnsubscribe(String subscriptionId, User user){
        String channel = "";
        try{
            user.lock(true);

            HashMap<String, String> userSubscriptions = user.getSubscriptions();
            if (!userSubscriptions.containsKey(subscriptionId))
                return "Missing subscription id";
            
            channel = userSubscriptions.get(subscriptionId);

            lock(channel, true);
            Set<User> channelUsers = channels.get(channel);
            channelUsers.remove(user);
            user.removeSubscription(subscriptionId);
        }
        finally{
            unlock(channel, true);
            user.unlock(true);
        }
        return "";
    }

    public String tryAddUser(String username, String passcode, int connectionId) {
        User user = users.getOrDefault(username, null);
        if (user == null){
            users.put(username, new User(username, passcode, connectionId));
            return "Login successful";
        }
        try{
            user.lock(true);
            if (!user.getPasscode().equals(passcode))
                return "Wrong password";
            else if (user.getConnected())
                return "User already logged in";
            else {
                user.toggleConnected();
                user.setConnectionId(connectionId);
            }
        }
        catch (Exception e){
            System.out.println(e);
        }
        finally{
            user.unlock(true);
        }
        return "Login successful";
    }

    public User getUser(String username) {
        return users.get(username);
    }

    public void removeUserFromChannels(User user){
        for (String channel : user.getSubscriptions().values()){
            try{
                lock(channel, true);
                channels.get(channel).remove(user);
            }
            finally{
                unlock(channel, true);
            }
        }
        user.clearSubscriptions();
    }

    public void lock(String channel, boolean write){
        if (write)
            channelsLocks.get(channel).writeLock().lock();
        else
            channelsLocks.get(channel).readLock().lock();
    }

    public void unlock(String channel, boolean write){
        if (write)
            channelsLocks.get(channel).writeLock().unlock();
        else
            channelsLocks.get(channel).readLock().unlock();

    }

}
