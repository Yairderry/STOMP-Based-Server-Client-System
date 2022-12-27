package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

import java.util.HashMap;
import java.util.List;

public class ConnectionsImpl<T> implements Connections<T> {

    HashMap<Integer, ConnectionHandler<T>> connections;
    HashMap<String, List<Integer>> channels;

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
        if (channels.containsKey(channel))
            for (Integer connectionId : channels.get(channel))
                connections.get(connectionId).send(msg);
    }

    @Override
    public void disconnect(int connectionId) {
        if (connections.containsKey(connectionId))
            connections.remove(connectionId);
    }
}
