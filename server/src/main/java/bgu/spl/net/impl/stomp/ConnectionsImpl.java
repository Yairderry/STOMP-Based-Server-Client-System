package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.database.Database;
import bgu.spl.net.impl.stomp.database.User;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

import java.util.HashMap;
import java.util.List;

public final class ConnectionsImpl<T> implements Connections<T> {

    private final HashMap<Integer, ConnectionHandler<T>> connections = new HashMap<>();

    private final Database db = new Database();

    int nextConnectionId = 0;

    @Override
    public boolean send(int connectionId, T msg) {
        if (connections.containsKey(connectionId)) {
            connections.get(connectionId).send(msg);
            return true;
        }
        return false;
    }

    @Override
    public void send(String channel, T msg) {
        List<User> channelUsers = db.getChannel(channel);
        if (channelUsers != null)
            for (User user : channelUsers)
                connections.get(user.getConnectionId()).send(msg);
    }

    @Override
    public void disconnect(int connectionId) {
        connections.remove(connectionId);
    }

    public void addConnection(ConnectionHandler<T> connectionHandler) {
        connections.put(nextConnectionId++, connectionHandler);
    }

    public Database getDB() {
        return db;
    }

}