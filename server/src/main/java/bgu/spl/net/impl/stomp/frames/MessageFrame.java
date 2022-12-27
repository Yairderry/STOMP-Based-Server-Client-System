package bgu.spl.net.impl.stomp.frames;

import java.util.HashMap;

public class MessageFrame extends Frame {
    public MessageFrame(String subscription, String messageId, String destination, String body) {
        super("MESSAGE", new HashMap<>(), body);
        this.headers.put("subscription", subscription);
        this.headers.put("message-id", messageId);
        this.headers.put("destination", destination);
    }
}
