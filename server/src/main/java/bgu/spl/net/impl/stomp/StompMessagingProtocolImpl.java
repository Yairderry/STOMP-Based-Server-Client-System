package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.database.Database;
import bgu.spl.net.impl.stomp.database.User;
import bgu.spl.net.impl.stomp.frames.ConnectedFrame;
import bgu.spl.net.impl.stomp.frames.ErrorFrame;
import bgu.spl.net.impl.stomp.frames.Frame;
import bgu.spl.net.impl.stomp.frames.ReceiptFrame;
import bgu.spl.net.srv.Connections;

import java.io.Serializable;
import java.util.HashMap;

public class StompMessagingProtocolImpl implements StompMessagingProtocol<String> {

    private boolean shouldTerminate = false;
    private int connectionId;
    private User connectedUser;
    private Connections<String> connections;

    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
        this.connectedUser = null;
    }

    @Override
    public void process(String message) {
        try{
            Frame frame = new Frame(message);
            String errorMessage = "Invalid command";
            switch (frame.getCommand()) {
                case "CONNECT":
                    errorMessage = Frame.isConnectFrame(frame);
                    if (errorMessage != null)
                        error(frame, errorMessage);
                    else
                        connect(frame);
                    break;
                case "DISCONNECT":
                    errorMessage = Frame.isDisconnectFrame(frame);
                    if (errorMessage != null)
                        error(frame, errorMessage);
                    else
                        disconnect(frame);
                    break;
                case "SUBSCRIBE":
                    errorMessage = Frame.isSubscribeFrame(frame);
                    if (errorMessage != null)
                        error(frame, errorMessage);
                    else
                        subscribe(frame);
                    break;
                case "UNSUBSCRIBE":
                    errorMessage = Frame.isUnsubscribeFrame(frame);
                    if (errorMessage != null)
                        error(frame, errorMessage);
                    else
//                    unsubscribe;
                        break;
                case "SEND":
                    errorMessage = Frame.isSendFrame(frame);
                    if (errorMessage != null)
                        error(frame, errorMessage);
                    else
                        send(frame);
                    break;
                default:
                    error(frame, errorMessage);
            }
        }
        catch (Exception e){
            error(new Frame(), "Something went wrong");
        }
    }

    private void connect(Frame frame) {
        HashMap<String, String> headers = frame.getHeaders();
        String message = connections.getDB().tryAddUser(headers.get("login"), headers.get("passcode"), connectionId);
        if (!message.equals("Login successful"))
            error(frame, message);
        else {
            connections.send(connectionId, new ConnectedFrame().toString());

            // Assign connected user
            this.connectedUser = connections.getDB().getUser(headers.get("login"));
        }
    }

    private void disconnect(Frame frame) {
        if (connectedUser == null)
            error(frame, "User is not connected");
        else {
            // send receipt
            connections.send(connectionId, new ReceiptFrame(frame.getHeaders().get("receipt-id")).toString());

            // remove user from channels and clear its subscriptions
            connections.getDB().removeUserFromChannels(connectedUser);

            // disconnect socket
            connections.disconnect(connectionId);
            shouldTerminate = true;
            connectedUser.toggleConnected();
            connectedUser = null;

        }
    }

    private void subscribe(Frame frame){
        if (connectedUser == null)
            error(frame, "User is not connected");
        else {
            // send receipt if needed
            String receiptId = frame.getHeaders().getOrDefault("receipt-id", null);
            if (receiptId != null)
                connections.send(connectionId, new ReceiptFrame(receiptId).toString());


        }
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
    private void send(Frame frame) {
        if (connectedUser == null)
            error(frame, "User is not connected");
        else {
            String channel = frame.getHeaders().get("destination");


            // send receipt if needed
            String receiptId = frame.getHeaders().getOrDefault("receipt-id", null);
            if (receiptId != null)
                connections.send(connectionId, new ReceiptFrame(receiptId).toString());


        }

    }

    private void error(Frame frame, String message) {
        HashMap<String, String> headers = frame.getHeaders();
        if (headers != null)
            connections.send(connectionId, new ErrorFrame(frame.getHeaders().getOrDefault("receipt-id", null), Frame.errorBody(frame, message)).toString());
        else
            connections.send(connectionId, new ErrorFrame(null, "").toString());
        connections.disconnect(connectionId);
        shouldTerminate = true;
//        connectedUser = null;
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
