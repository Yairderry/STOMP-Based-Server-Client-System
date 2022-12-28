package bgu.spl.net.impl.stomp.frames;

import java.util.HashMap;

public class ErrorFrame extends Frame {

    public ErrorFrame(String receiptId, String body) {
        super("ERROR", new HashMap<>(), body);
        this.headers.put("message", "malformed frame received");
        if (receiptId != null)
            this.headers.put("receipt-id", receiptId);
    }
}
