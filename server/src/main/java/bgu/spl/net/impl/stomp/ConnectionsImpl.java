package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

import java.util.HashMap;
import java.util.concurrent.atomic.AtomicInteger;

public final class ConnectionsImpl<T> implements Connections<T> {

    private final HashMap<Integer, ConnectionHandler<T>> connections = new HashMap<>();
    AtomicInteger nextConnectionId = new AtomicInteger(0);

    @Override
    public boolean send(int connectionId, T msg) {        
        if (connections.containsKey(connectionId)) {
            ConnectionHandler<T> handler = connections.get(connectionId);
            synchronized(handler){
                handler.send(msg);
                return true;
            }
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
        connections.put(nextConnectionId.get(), connectionHandler);
        protocol.start(nextConnectionId.getAndIncrement(), this);
    }

}