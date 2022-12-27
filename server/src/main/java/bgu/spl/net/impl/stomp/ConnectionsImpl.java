package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

import java.util.List;

public class ConnectionsImpl implements Connections<String> {

    List<ConnectionHandler<String>> connections;

    @Override
    public boolean send(int connectionId, String msg) {
        return false;
    }

    @Override
    public void send(String channel, String msg) {

    }

    @Override
    public void disconnect(int connectionId) {

    }
}
}
