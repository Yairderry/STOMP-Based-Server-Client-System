package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

import java.util.HashMap;

public class ConnectionsImpl<T> implements Connections<T> {

    HashMap<Integer, ConnectionHandler<T>> connections;

    @Override
    public boolean send(int connectionId, T msg) {
        if (connections.containsKey(connectionId)) {
            connections.get(connectionId).send(msg);
            return true;
        }
    }

    @Override
    public void send(String channel, T msg) {

    }

    @Override
    public void disconnect(int connectionId) {

    }
}
