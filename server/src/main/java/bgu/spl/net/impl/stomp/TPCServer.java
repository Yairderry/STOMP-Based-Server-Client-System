package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.BaseServer;
import bgu.spl.net.srv.BlockingConnectionHandler;

import java.util.function.Supplier;

public class TPCServer<T> extends BaseServer<T> {
    public TPCServer(int port) {
        super(port, new Supplier<MessagingProtocol<T>>, new Supplier<MessageEncoderDecoder<T>>);
    }

    @Override
    protected void execute(BlockingConnectionHandler<T> handler) {

    }
}
