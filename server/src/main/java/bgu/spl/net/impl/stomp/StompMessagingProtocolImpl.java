package bgu.spl.net.impl.stomp;

import java.util.HashSet;
import java.util.Set;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.database.Database;
import bgu.spl.net.impl.stomp.database.User;
import bgu.spl.net.impl.stomp.frames.*;
import bgu.spl.net.srv.Connections;

public class StompMessagingProtocolImpl implements StompMessagingProtocol<String> {

    private boolean shouldTerminate = false;
    private int connectionId;
    private User connectedUser;
    private Connections<String> connections;
    private final Database db = new Database();
    private int nextMessageId = 0;

    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
        this.connectedUser = null;
    }

    @Override
    public void process(String message) {
        try {
            Frame frame = new Frame(message.trim());
            System.out.println("/////////////Received Frame/////////////");
            System.out.println(frame);
            System.out.println("////////////////////////////////////////");
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
                        unsubscribe(frame);
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
        } catch (Exception e) {
            error(new Frame(), "Something went wrong");
        }
    }

    private void connect(Frame frame) {
        String message = db.tryAddUser(frame.getHeader("login"), frame.getHeader("passcode"), connectionId);
        if (!message.equals("Login successful"))
            error(frame, message);
        else {
            connections.send(connectionId, new ConnectedFrame().toString());

            // Assign connected user
            this.connectedUser = db.getUser(frame.getHeader("login"));
        }
    }

    private void disconnect(Frame frame) {
        if (acknowledge(frame))
            return;

        try{
            connectedUser.lock(true);

            // remove user from channels and clear its subscriptions
            db.removeUserFromChannels(connectedUser);
    
            // disconnect socket
            shouldTerminate = true;
            connectedUser.toggleConnected();
        }
        finally{
            connectedUser.unlock(true);
        }

        connectedUser = null;
    }

    private void subscribe(Frame frame) {
        if (acknowledge(frame))
            return;
        String message = db.trySubscribe(frame.getHeader("id"), frame.getHeader("destination"), connectedUser);
        if (!message.equals(""))
            error(frame, message);
    }

    private void unsubscribe(Frame frame) {
        if (acknowledge(frame))
            return;
        String message = db.tryUnsubscribe(frame.getHeader("id"), connectedUser);
        if (!message.equals(""))
            error(frame, message);
    }

    private void send(Frame frame) {
        if (acknowledge(frame))
            return;
        
        String channel = frame.getHeader("destination");
        Set<User> subscribedUsers = new HashSet<User>();
        try{
            connectedUser.lock(false);
            db.lock(channel, false);

            String subscriptionId = connectedUser.getSubscriptionId(channel);

            // Validate subscription
            if (subscriptionId == null) {
                error(frame, "User is not subscribed to this channel");
                return;
            }
            
            subscribedUsers = db.getChannel(channel);

            for (User user : subscribedUsers){
                if (user.getConnectionId() != connectionId) user.lock(false);
                // Generate and send message frame to the channel
                Frame messageFrame = new MessageFrame(user.getSubscriptionId(channel), "" + nextMessageId++, channel, frame.getBody());
                connections.send(user.getConnectionId(), messageFrame.toString());
            }
        }
        finally{
            db.unlock(channel, false);
            for (User user : subscribedUsers)
                user.unlock(false);
        }


    }

    private void error(Frame frame, String message) {
        if (frame.getHeaders() != null)
            connections.send(connectionId, new ErrorFrame(frame.getHeader("receipt-id"), Frame.errorBody(frame, message)).toString());
        else
            connections.send(connectionId, new ErrorFrame(null, "").toString());
        connections.disconnect(connectionId);
        shouldTerminate = true;
//        connectedUser = null;
    }

    private void receipt(Frame frame) {
        String receiptId = frame.getHeader("receipt-id");
        if (receiptId != null){
            connections.send(connectionId, new ReceiptFrame(receiptId).toString());
        }
    }

    private boolean acknowledge(Frame frame){
        if (connectedUser == null) {
            error(frame, "User is not connected");
            return true;
        }
        else {
            // send receipt if needed
            receipt(frame);
            return false;
        }
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
