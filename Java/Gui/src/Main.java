import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        JFrame window = new JFrame();
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JTextField tf;
        tf = new JTextField();
        int tf1=100, tf2=0, tf3=500, tf4=100;
        tf.setBounds(tf1,tf2,tf3,tf4);
        tf.setFont(new Font("Arial", Font.PLAIN, 50));

        JTextArea area = new JTextArea();
        int area1=0, area2=100, area3=600, area4=500;
        area.setBounds(area1,area2,area3,area4);
        area.setFont(new Font("Courier New", Font.PLAIN, 20));
        window.add(area);

        JButton button = new JButton("run");
        button.setBounds(0,0,100,100);
        button.setFont(new Font("Arial", Font.PLAIN, 40));
        button.setForeground(Color.BLUE);
        button.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                String s = tf.getText();
                if (s == null || s.isEmpty()) {
                    System.out.println("Pole tekstowe jest puste");
                    return;
                }
                try {
                    List<String> command = new ArrayList<>();
                    command.add("src/program.exe");
                    command.addAll(Arrays.asList(s.split("\\s+")));
                    ProcessBuilder builder = new ProcessBuilder(command);
                    Process process = builder.start();
                    BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                    String line, wynik ="";
                    while ((line = reader.readLine()) != null) {wynik += line + "\n";}
                    area.setText(wynik);
                    System.out.println(wynik);
                } catch (Exception ex) {ex.printStackTrace();}
        }});

        window.add(button);window.add(tf);
        int window1 = 600,window2 = 600;
        window.setSize(window1, window2);
        window.setLayout(null);
        window.setVisible(true);

    }
}
