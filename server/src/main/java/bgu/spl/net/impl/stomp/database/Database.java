package bgu.spl.net.impl.stomp.database;

import java.util.*;

public class Database {

    // key: channel-name , value: list of users subscribed to the channel
    private final HashMap<String, Set<User>> channels = new HashMap<>();

    // key: username , value: user
    private final HashMap<String, User> users = new HashMap<>();

    public Set<User> getChannel(String channel) {
        return channels.getOrDefault(channel, null);
    }

    public String trySubscribe(String subscriptionId, String channel, User user){
        channels.putIfAbsent(channel, new HashSet<User>());
        Set<User> channelUsers = channels.get(channel);
        if (channelUsers.contains(user))
            return "User already subscribed to this topic";
        channelUsers.add(user);
        user.addSubscription(subscriptionId, channel);
        return "";
    }

    public String tryUnsubscribe(String subscriptionId, User user){
        HashMap<String, String> userSubscriptions = user.getSubscriptions();
        if (!userSubscriptions.containsKey(subscriptionId))
            return "Missing subscription id";
        Set<User> channel = channels.get(userSubscriptions.get(subscriptionId));
        channel.remove(user);
        user.removeSubscription(subscriptionId);
        return "";
    }

    public String tryAddUser(String username, String passcode, int connectionId) {
        User user = users.getOrDefault(username, null);
        if (user == null)
            users.put(username, new User(username, passcode, connectionId));
        else if (!user.getPasscode().equals(passcode))
            return "Wrong password";
        else if (user.getConnected())
            return "User already logged in";
        else {
            user.toggleConnected();
            user.setConnectionId(connectionId);
        }
        return "Login successful";
    }

    public User getUser(String username) {
        return users.get(username);
    }

    public void removeUserFromChannels(User user){
        for (String channel : user.getSubscriptions().values())
            channels.get(channel).remove(user);
        user.clearSubscriptions();
    }
}
