package bgu.spl.net.impl.stomp.frames;

import java.util.HashMap;

public class ConnectedFrame extends Frame {
    private final String version = "1.2";

    public ConnectedFrame() {
        super("CONNECTED", new HashMap<>(), "");
        this.headers.put("version", version);
    }
}
