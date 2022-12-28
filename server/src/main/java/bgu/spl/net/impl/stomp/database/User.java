package bgu.spl.net.impl.stomp.database;

public class User {
    private final String username;
    private final String passcode;
    private boolean connected = true;
    private int connectionId;

    public User(String _username, String _passcode, int _connectionId){
        this.username = _username;
        this.passcode = _passcode;
        this.connectionId = _connectionId;
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
}
