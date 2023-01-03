package bgu.spl.net.impl.stomp.database;

import java.util.HashMap;

public class User {
    private final String username;
    private final String passcode;
    private boolean connected = true;

    // key: subscription id, value: channel name
    private final HashMap<String, String> subscriptions;
    private int connectionId;

    public User(String _username, String _passcode, int _connectionId){
        this.username = _username;
        this.passcode = _passcode;
        this.connectionId = _connectionId;
        this.subscriptions = new HashMap<>();
    }

    public String getPasscode(){
        return passcode;
    }

    public boolean getConnected(){
        return connected;
    }

    public void toggleConnected(){
        connected = !connected;
    }

    public int getConnectionId(){
        return connectionId;
    }

    public void setConnectionId(int _connectionId){
        this.connectionId = _connectionId;
    }

    public void addSubscription(String subscriptionId, String channel){
        subscriptions.put(subscriptionId, channel);
    }

    public void removeSubscription(String subscriptionId){
        subscriptions.remove(subscriptionId);
    }

    public HashMap<String, String> getSubscriptions(){
        return subscriptions;
    }

    public void clearSubscriptions(){
        subscriptions.clear();
    }
}
