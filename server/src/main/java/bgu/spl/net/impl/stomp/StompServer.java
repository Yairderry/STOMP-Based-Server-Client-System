package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Usage: StompServer <port> <server type>");
            return;
        }
        if (args[1].equals("tpc"))
            Server.threadPerClient(Integer.parseInt(args[0]), StompMessagingProtocolImpl::new, MessageEncoderDecoderImpl::new).serve();
        else if (args[1].equals("reactor"))
            Server.reactor(Runtime.getRuntime().availableProcessors(), Integer.parseInt(args[0]), StompMessagingProtocolImpl::new, MessageEncoderDecoderImpl::new).serve();
    }
}
