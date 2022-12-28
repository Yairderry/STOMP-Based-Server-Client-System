package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.frames.ConnectedFrame;
import bgu.spl.net.impl.stomp.frames.ErrorFrame;
import bgu.spl.net.impl.stomp.frames.Frame;
import bgu.spl.net.srv.Connections;

import java.io.Serializable;
import java.util.HashMap;

public class StompMessagingProtocolImpl implements StompMessagingProtocol {

    private boolean shouldTerminate = false;
    private int connectionId;
    private Connections<Serializable> connections;

    @Override
    public void start(int connectionId, Connections<Serializable> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }

    @Override
    public void process(Serializable message) {
        Frame frame = (Frame) message;
        String error = "Invalid command";
        switch (frame.getCommand()) {
            case "CONNECT":
                error = Frame.isConnectFrame(frame);
                if (error != null)
                    connections.send(connectionId, new ErrorFrame(frame.getHeaders().getOrDefault("receipt-id", null), Frame.errorBody(frame, error)).toString());
                else
                    connect(frame);
                break;
            case "DISCONNECT":
                error = Frame.isDisconnectFrame(frame);
                if (error != null)
                    connections.send(connectionId, new ErrorFrame(frame.getHeaders().getOrDefault("receipt-id", null), Frame.errorBody(frame, error)));
//                else
//                    disconnect;
                break;
            case "SUBSCRIBE":
                error = Frame.isSubscribeFrame(frame);
                if (error != null)
                    connections.send(connectionId, new ErrorFrame(frame.getHeaders().getOrDefault("receipt-id", null), Frame.errorBody(frame, error)).toString());
//                else
//                    subscribe(frame);
                break;
            case "UNSUBSCRIBE":
                error = Frame.isUnsubscribeFrame(frame);
                if (error != null)
                    connections.send(connectionId, new ErrorFrame(frame.getHeaders().getOrDefault("receipt-id", null), Frame.errorBody(frame, error)));
//                else
//                    unsubscribe;
                break;
            case "SEND":
                error = Frame.isSendFrame(frame);
                if (error != null)
                    connections.send(connectionId, new ErrorFrame(frame.getHeaders().getOrDefault("receipt-id", null), Frame.errorBody(frame, error)));
//                else
//                    send;
                break;
            default:
                connections.send(connectionId, new ErrorFrame(frame.getHeaders().getOrDefault("receipt-id", null), Frame.errorBody(frame, error)));
        }
    }

    private void connect(Frame frame) {
        HashMap<String, String> headers = frame.getHeaders();
        String message = connections.getDB().tryAddUser(headers.get("login"), headers.get("passcode"), connectionId);
        if (!message.equals("Login successful"))
            connections.send(connectionId, new ErrorFrame(frame.getHeaders().getOrDefault("receipt-id", null), Frame.errorBody(frame, message)));
        else
            connections.send(connectionId, new ConnectedFrame());
    }

    private void disconnect(Frame frame){

    }
//
//    private void subscribe(String destination, String id, String receipt) {
//        if (connections.subscribe(destination, id, connectionId)) {
//            connections.send(connectionId, "RECEIPT\nreceipt-id:" + receipt + "\n\n\0");
//        } else {
//            connections.send(connectionId, "ERROR\nmessage:Already subscribed to " + destination + "\n\n\0");
//        }
//    }
//
//    private void unsubscribe(String id) {
//        if (connections.unsubscribe(id, connectionId)) {
//            connections.send(connectionId, "RECEIPT\nreceipt-id:" + id + "\n\n\0");
//        } else {
//            connections.send(connectionId, "ERROR\nmessage:Not subscribed to " + id + "\n\n\0");
//        }
//    }
//
//    private void send(String destination, String receipt, String[] splitMessage) {
//        String body = "";
//        for (int i = 1; i < splitMessage.length; i++) {
//            body += splitMessage[i] + "\n";
//        }
//    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
