package bgu.spl.net.impl.stomp.frames;

import java.util.HashMap;

public class ErrorFrame extends Frame {

    public ErrorFrame(String receiptId, String message, String body) {
        super("ERROR", new HashMap<>(), body);
        this.headers.put("receipt-id", receiptId);
        this.headers.put("message", message);
    }

    public static String errorBody(Frame frame, String message) {
        return "The message:" + "\n-----\n" + frame.formatFrame(false) + "\n-----\n" + message;
    }
}
