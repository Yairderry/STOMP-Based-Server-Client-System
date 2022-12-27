package bgu.spl.net.impl.stomp.frames;

import java.util.Arrays;
import java.util.HashMap;

public class Frame {
    private final String command;
    protected final HashMap<String, String> headers;
    private final String body;
    private final String endOfLine = "\u0000";

    public Frame(String command, HashMap<String, String> headers, String body) {
        this.command = command;
        this.headers = headers;
        this.body = body;
    }

    public Frame(String frame) {
        String[] lines = frame.split("\n");
        this.command = lines[0];
        this.headers = new HashMap<>();

        int startOfBody = 2;
        for (int i = 1; i < lines.length; i++) {
            if (lines[i].length() == 0) {
                startOfBody = i + 1;
                break;
            }
            String[] header = lines[i].split(":");
            this.headers.put(header[0], header[1]);
        }

        this.body = String.join("\n", Arrays.copyOfRange(lines, startOfBody, lines.length - 1));
    }

    public String formatFrame(boolean addEndOfLine) {
        StringBuilder frame = new StringBuilder(command + "\n");
        for (String key : headers.keySet())
            frame.append(key).append(":").append(headers.get(key)).append("\n");
        frame.append("\n").append(body).append(addEndOfLine ? endOfLine : "");
        return frame.toString();
    }
}
