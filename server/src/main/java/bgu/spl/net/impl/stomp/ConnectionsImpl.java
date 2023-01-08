package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

import java.util.HashMap;

public final class ConnectionsImpl<T> implements Connections<T> {

    private final HashMap<Integer, ConnectionHandler<T>> connections = new HashMap<>();
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
        return;
    }

    @Override
    public void disconnect(int connectionId) {
        connections.remove(connectionId);
    }

    public void addConnection(ConnectionHandler<T> connectionHandler, StompMessagingProtocol<T> protocol) {
        connections.put(nextConnectionId, connectionHandler);
        protocol.start(nextConnectionId++, this);
    }

}