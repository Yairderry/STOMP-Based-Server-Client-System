package bgu.spl.net.impl.stomp.database;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class Database {

    // key: channel-name , value: list of users subscribed to the channel
    private final HashMap<String, List<User>> channels = new HashMap<>();

    // key: username , value: user
    private final HashMap<String, User> users = new HashMap<>();

    public List<User> getChannel(String channel) {
        return channels.getOrDefault(channel, null);
    }

    public void addChannel(String channel) {
        channels.putIfAbsent(channel, new ArrayList<User>());
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
