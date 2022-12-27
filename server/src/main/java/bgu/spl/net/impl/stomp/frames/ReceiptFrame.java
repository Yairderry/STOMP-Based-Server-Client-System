package bgu.spl.net.impl.stomp.frames;

import java.util.HashMap;

public class ReceiptFrame extends Frame {
    public ReceiptFrame(String receiptId) {
        super("RECEIPT", new HashMap<>(), "");
        this.headers.put("receipt-id", receiptId);
    }
}
