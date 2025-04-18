import javax.swing.*;
import java.awt.event.*;
import java.awt.Font;
import java.awt.Color;

class PascalTriangle {
    int n;

    public PascalTriangle(int n) {
        this.n = n;
    }

    public String getResult() {
        if (n == 1) {
            return "1";
        }

        int[][] t = new int[n][];
        for (int i = 0; i < n; i++) {
            t[i] = new int[i + 1];
            for (int j = 0; j < i + 1; j++) {
                if (j == 0 || j == i) {
                    t[i][j] = 1;
                } else {
                    t[i][j] = t[i - 1][j - 1] + t[i - 1][j];
                }
            }
        }

        String result = "";
        String lastRow = "";
        for (int i = 0; i < n; i++) {
            lastRow += Integer.toString(t[n - 1][i]) + " ";
        }
        int lastRowLength = lastRow.length();
        for (int i = 0; i < n; i++) {
            String row = "";
            for (int j = 0; j < i + 1; j++) {
                row += Integer.toString(t[i][j]) + " ";
            }
            int padding = (lastRowLength - row.length()) / 2;
            String tmp = "";
            for (int x = 0; x < padding; x++) {
                tmp += " ";
            }
            tmp += row;
            result += tmp + "\n";
        }
        return result;
    }
}

public class Main {
    public static void main(String[] args) {
        JFrame window = new JFrame();
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JTextField tf = new JTextField();
        int tf1 = 100, tf2 = 0, tf3 = 400, tf4 = 100;
        tf.setBounds(tf1, tf2, tf3, tf4);
        tf.setFont(new Font("Arial", Font.PLAIN, 50));

        JTextArea area = new JTextArea();
        int area1 = 0, area2 = 0, area3 = 0, area4 = 0;
        area.setBounds(area1, area2, area3, area4);
        area.setFont(new Font("Courier New", Font.PLAIN, 12));
        window.add(area);

        JButton button = new JButton("run");
        button.setBounds(0, 0, 100, 100);
        button.setFont(new Font("Arial", Font.PLAIN, 40));
        button.setForeground(Color.BLUE);
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String s = tf.getText();
                if (s == null || s.isEmpty()) {
                    System.out.println("Text field is empty");
                    return;
                }
                try {
                    int n = Integer.parseInt(s);
                    if (n < 1) {
                        System.out.println("n is too small");
                        return;
                    }
                    PascalTriangle triangle = new PascalTriangle(n);
                    String result = triangle.getResult();
                    int windowX = 27 * n * 2, windowY = 22 * n;
                    if (windowX < 400 && windowY < 400) {
                        window.setSize(400, 400);
                        area.setBounds(0, 100, 400, 300);
                        tf.setBounds(100, 0, 300, 100);
                        area.setText(result);
                    } else if (windowX < 400 && windowY >= 400) {
                        window.setSize(400, windowY);
                        area.setBounds(0, 100, 400, windowY - 100);
                        tf.setBounds(100, 0, 300, 100);
                        area.setText(result);
                    } else if (windowX >= 400 && windowY < 400) {
                        window.setSize(windowX, 400);
                        area.setBounds(0, 100, windowX, 300);
                        tf.setBounds(100, 0, windowX - 100, 100);
                        area.setText(result);
                    } else {
                        window.setSize(windowX, windowY);
                        area.setBounds(0, 100, windowX, windowY - 100);
                        tf.setBounds(100, 0, windowX - 100, 100);
                        area.setText(result);
                    }
                } catch (NumberFormatException ex) {
                    System.out.println("Invalid number format");
                }
            }
        });

        window.add(button);
        window.add(tf);
        int window1 = 400, window2 = 400;
        window.setSize(window1, window2);
        window.setLayout(null);
        window.setVisible(true);
    }
}