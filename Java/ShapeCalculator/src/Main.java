abstract class Figury {
    public interface dwaParametry {
        double ObliczPole();
        double ObliczObwod();
        String PodajNazwe();
    }
    public interface jedenParametr {
        double ObliczPole();
        double ObliczObwod();
        String PodajNazwe();
    }

    public enum figuryJeden implements jedenParametr {
        Kolo {
            public double ObliczPole() {
                return Math.PI * parametr * parametr;
            }
            public double ObliczObwod() {
                return 2 * Math.PI * parametr;
            }
            public String PodajNazwe() {
                return "Koło";
            }
        },
        Kwadrat {
            public double ObliczPole() {
                return parametr * parametr;
            }
            public double ObliczObwod() {
                return 4 * parametr;
            }
            public String PodajNazwe() {
                return "Kwadrat";
            }
        },
        Pieciokat {
            public double ObliczPole() {
                return 0.25 * Math.sqrt(5 * (5 + 2 * Math.sqrt(5))) * parametr * parametr;
            }
            public double ObliczObwod() {
                return 5 * parametr;
            }
            public String PodajNazwe() {
                return "Pieciokat";
            }
        },
        Szeciokat {
            public double ObliczPole() {
                return (3 * Math.sqrt(3) * parametr * parametr) / 2;
            }
            public double ObliczObwod() {
                return 6 * parametr;
            }
            public String PodajNazwe() {
                return "Szeciokat";
            }
        };
        protected double parametr;
        public void ustawParametr(double parametr) {
            this.parametr = parametr;
        }
    }
    public enum figuryDwa implements dwaParametry{
        Prostokat {
            public double ObliczPole() {
                return par1 * par2;
            }
            public double ObliczObwod() {
                return 2 * par1 + 2 * par2;
            }
            public String PodajNazwe() {
                return "Prostokat";
            }
        },
        Romb {
            public double ObliczPole() {
                return par1 * par1 * Math.sin(Math.toRadians(par2));
            }
            public double ObliczObwod() {
                return 4 * par1;
            }
            public String PodajNazwe() {
                return "Prostokat";
            }
        };
        protected double par1;
        protected double par2;
        public void ustawParametr2(double par1, double par2) {
            this.par1 = par1;
            this.par2 = par2;
        }
    }
}

public class Main {
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Za mało danych");
            return;
        }

        try {
            switch (args[0]) {
                case "o":
                    Figury.figuryJeden kolo = Figury.figuryJeden.Kolo;
                    kolo.ustawParametr(Double.parseDouble(args[1]));
                    System.out.println(kolo.PodajNazwe());
                    System.out.println("Pole: " + kolo.ObliczPole());
                    System.out.println("Obwód: " + kolo.ObliczObwod());
                    break;
                case "p":
                    Figury.figuryJeden pieciokat = Figury.figuryJeden.Pieciokat;
                    pieciokat.ustawParametr(Double.parseDouble(args[1]));
                    System.out.println(pieciokat.PodajNazwe());
                    System.out.println("Pole: " + pieciokat.ObliczPole());
                    System.out.println("Obwód: " + pieciokat.ObliczObwod());
                    break;
                case "s":
                    Figury.figuryJeden szesciokat = Figury.figuryJeden.Szeciokat;
                    szesciokat.ustawParametr(Double.parseDouble(args[1]));
                    System.out.println(szesciokat.PodajNazwe());
                    System.out.println("Pole: " + szesciokat.ObliczPole());
                    System.out.println("Obwód: " + szesciokat.ObliczObwod());
                    break;
                case "c":
                    if(Double.parseDouble(args[5]) != 90){
                        if(Double.parseDouble(args[1]) == Double.parseDouble(args[2]) && Double.parseDouble(args[2]) == Double.parseDouble(args[3]) && Double.parseDouble(args[3]) == Double.parseDouble(args[4])){
                            Figury.figuryDwa romb = Figury.figuryDwa.Romb;
                            romb.ustawParametr2(Double.parseDouble(args[1]), Double.parseDouble(args[5]));
                            System.out.println(romb.PodajNazwe());
                            System.out.println("Pole: " + romb.ObliczPole());
                            System.out.println("Obwód: " + romb.ObliczObwod());
                        }
                        else{
                            System.out.println("Zle wymiary bokow rombu");
                        }
                    } else if(Double.parseDouble(args[1]) == Double.parseDouble(args[2]) && Double.parseDouble(args[2]) == Double.parseDouble(args[3]) && Double.parseDouble(args[3]) == Double.parseDouble(args[4])){
                        Figury.figuryJeden kwadrat = Figury.figuryJeden.Kwadrat;
                        kwadrat.ustawParametr(Double.parseDouble(args[1]));
                        System.out.println(kwadrat.PodajNazwe());
                        System.out.println("Pole: " + kwadrat.ObliczPole());
                        System.out.println("Obwód: " + kwadrat.ObliczObwod());
                    } else if(Double.parseDouble(args[1]) == Double.parseDouble(args[2]) && Double.parseDouble(args[3]) == Double.parseDouble(args[4])){
                        Figury.figuryDwa prostokat = Figury.figuryDwa.Prostokat;
                        prostokat.ustawParametr2(Double.parseDouble(args[1]), Double.parseDouble(args[4]));
                        System.out.println(prostokat.PodajNazwe());
                        System.out.println("Pole: " + prostokat.ObliczPole());
                        System.out.println("Obwód: " + prostokat.ObliczObwod());
                    }else{
                        System.out.println("Zle wymiary bokow");
                    }
                    break;
                default:
                    System.out.println("Zle dane");
                    break;
            }
        } catch (NumberFormatException ex) {
            System.out.println("Nieprawidłowe dane wejściowe");
        }
    }
}
