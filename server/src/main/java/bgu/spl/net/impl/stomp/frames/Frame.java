package bgu.spl.net.impl.stomp.frames;

import java.io.Serializable;
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

    public Frame(){
        this.command = null;
        this.headers = null;
        this.body = null;
    }

    public Frame(String frame) {
        String[] lines = frame.split("\\n", -1);
        this.command = lines[0];
        this.headers = new HashMap<>();

        int startOfBody = 2;
        for (int i = 1; i < lines.length; i++) {
            if (lines[i].equals("")) {
                startOfBody = i + 1;
                break;
            }
            String[] header = lines[i].split(":");
            this.headers.put(header[0], header[1]);
        }
        this.body = String.join("\n", Arrays.copyOfRange(lines, startOfBody, lines.length-1));
    }

    @Override
    public String toString() {
        StringBuilder frame = new StringBuilder(command).append("\n");
        for (String key : headers.keySet())
            frame.append(key).append(":").append(headers.get(key)).append("\n");
        frame.append("\n"); // gap
        frame.append(body.equals("") ? "" : body + "\n").append(endOfLine);
        return frame.toString();
    }

    public String getCommand() {
        return command;
    }

    public HashMap<String, String> getHeaders(){
        return headers;
    }

    public String getHeader(String header){
        return headers.getOrDefault(header, null);
    }

    public String getBody(){
        return body;
    }

    public static String errorBody(Frame frame, String bodyMessage) {
        return "The message:" + "\n-----\n" + frame.toString().replace("\n\u0000", "") + "\n-----\n" + bodyMessage;
    }

    public static String isConnectFrame(Frame frame) {
        if (!frame.headers.containsKey("accept-version"))
            return "accept-version header is missing";
        if (!frame.headers.get("accept-version").equals("1.2"))
            return "accept-version header is not 1.2";
        if (!frame.headers.containsKey("host"))
            return "host header is missing";
        // if (!frame.headers.get("host").equals("1.2"))
        //     return "host header is not stomp.cs.bgu.ac.il";
        if (!frame.headers.containsKey("login"))
            return "login header is missing";
        if (!frame.headers.containsKey("passcode"))
            return "passcode header is missing";
        return null;
    }

    public static String isDisconnectFrame(Frame frame) {
        if (!frame.headers.containsKey("receipt"))
            return "receipt header is missing";
        if (!frame.headers.get("receipt").matches("[0-9]+"))
            return "receipt header is not a number";
        return null;
    }

    public static String isSubscribeFrame(Frame frame) {
        if (!frame.headers.containsKey("destination"))
            return "destination header is missing";
        if (!frame.headers.containsKey("id"))
            return "id header is missing";
        if (!frame.headers.get("id").matches("[0-9]+"))
            return "id header is not a number";
        return null;
    }

    public static String isUnsubscribeFrame(Frame frame) {
        if (!frame.headers.containsKey("id"))
            return "id header is missing";
        if (!frame.headers.get("id").matches("[0-9]+"))
            return "id header is not a number";
        return null;
    }

    public static String isSendFrame(Frame frame) {
        if (!frame.headers.containsKey("destination"))
            return "destination header is missing";
        return null;
    }
}
