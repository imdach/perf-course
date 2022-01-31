import com.lmax.disruptor.RingBuffer;
import com.lmax.disruptor.dsl.Disruptor;
import com.lmax.disruptor.util.DaemonThreadFactory;

import java.nio.ByteBuffer;
import java.util.concurrent.TimeUnit;

/**
 * Created By Arthur Zhang at 2022/1/20
 */
public class DisruptorTest {
    static class MyEvent {
        private long value;

        public void set(long value) {
            this.value = value;
        }

        @Override
        public String toString() {
            return "LongEvent{" + "value=" + value + '}';
        }
    }

    public static void main(String[] args) throws InterruptedException {
        // 指定 RingBuffer 大小,
        int bufferSize = 1024;

        // 构建 Disruptor
        Disruptor<MyEvent> disruptor = new Disruptor<>(MyEvent::new, bufferSize, DaemonThreadFactory.INSTANCE);

        // 注册事件处理器
        disruptor.handleEventsWith((event, sequence, endOfBatch) -> System.out.println("Receive event: " + event));

        // 启动 Disruptor
        disruptor.start();

        // 获取 RingBuffer
        RingBuffer<MyEvent> ringBuffer = disruptor.getRingBuffer();
        // 生产 Event
        ByteBuffer bb = ByteBuffer.allocate(8);
        for (long l = 0; ; l++) {
            bb.putLong(0, l);
            // 生产者生产消息
            ringBuffer.publishEvent((event, sequence, buffer) -> event.set(buffer.getLong(0)), bb);
            TimeUnit.MILLISECONDS.sleep(1000);
        }
    }
}
