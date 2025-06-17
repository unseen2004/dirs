class FrameProcessor:
    def __init__(self, crc_polynomial='11010101'):
        """
        Inicjalizacja procesora ramek
        :param crc_polynomial: Wielomian CRC (domyślnie CRC-8)
        """
        self.flag = '01111110'  # Flaga początku/końca ramki
        self.crc_poly = crc_polynomial
        self.stuff_bits = 5  # Po 5 jedynkach wstawiamy 0

    def calculate_crc(self, data):
        """
        Oblicza CRC dla danych
        :param data: Ciąg bitów (string '0' i '1')
        :return: CRC jako string bitów
        """
        # Dodajemy zera na końcu (długość wielomianu - 1)
        padded_data = data + '0' * (len(self.crc_poly) - 1)

        # Konwertujemy na listy dla łatwiejszej manipulacji
        dividend = list(padded_data)
        divisor = list(self.crc_poly)

        # Dzielenie modulo 2
        for i in range(len(data)):
            if dividend[i] == '1':
                for j in range(len(divisor)):
                    dividend[i + j] = str((int(dividend[i + j]) + int(divisor[j])) % 2)

        # CRC to reszta z dzielenia
        remainder = ''.join(dividend[-(len(self.crc_poly) - 1):])
        return remainder

    def bit_stuffing(self, data):
        """
        Wykonuje bit stuffing - po 5 jedynkach wstawia 0
        :param data: Ciąg bitów
        :return: Ciąg po bit stuffing
        """
        result = []
        ones_count = 0

        for bit in data:
            result.append(bit)
            if bit == '1':
                ones_count += 1
                if ones_count == self.stuff_bits:
                    result.append('0')  # Wstawiamy 0 po 5 jedynkach
                    ones_count = 0
            else:
                ones_count = 0

        return ''.join(result)

    def bit_destuffing(self, data):
        """
        Usuwa wstawione bity (destuffing)
        :param data: Ciąg po bit stuffing
        :return: Oryginalny ciąg
        """
        result = []
        ones_count = 0
        i = 0

        while i < len(data):
            result.append(data[i])
            if data[i] == '1':
                ones_count += 1
                if ones_count == self.stuff_bits:
                    # Pomijamy następne 0 (było wstawione)
                    if i + 1 < len(data) and data[i + 1] == '0':
                        i += 1
                    ones_count = 0
            else:
                ones_count = 0
            i += 1

        return ''.join(result)

    def create_frame(self, data):
        """
        Tworzy ramkę z danych
        :param data: Dane do umieszczenia w ramce
        :return: Kompletna ramka
        """
        # Obliczamy CRC dla danych
        crc = self.calculate_crc(data)

        # Łączymy dane z CRC
        frame_content = data + crc

        # Wykonujemy bit stuffing na zawartości ramki
        stuffed_content = self.bit_stuffing(frame_content)

        # Dodajemy flagi początku i końca
        frame = self.flag + stuffed_content + self.flag

        return frame

    def extract_frame(self, frame):
        """
        Ekstraktuje dane z ramki i weryfikuje CRC
        :param frame: Ramka do przetworzenia
        :return: (dane, czy_poprawne) lub (None, False) jeśli błąd
        """
        # Usuwamy flagi
        if not (frame.startswith(self.flag) and frame.endswith(self.flag)):
            return None, False

        content = frame[len(self.flag):-len(self.flag)]

        # Usuwamy bit stuffing
        destuffed_content = self.bit_destuffing(content)

        # Dzielimy na dane i CRC
        crc_len = len(self.crc_poly) - 1
        if len(destuffed_content) < crc_len:
            return None, False

        data = destuffed_content[:-crc_len]
        received_crc = destuffed_content[-crc_len:]

        # Weryfikujemy CRC
        calculated_crc = self.calculate_crc(data)

        if calculated_crc == received_crc:
            return data, True
        else:
            return None, False

    def encode_file(self, input_file, output_file, frame_size=64):
        """
        Koduje plik źródłowy do pliku z ramkami
        :param input_file: Nazwa pliku wejściowego
        :param output_file: Nazwa pliku wyjściowego
        :param frame_size: Rozmiar danych w ramce (w bitach)
        """
        try:
            with open(input_file, 'r') as f:
                data = f.read().strip()

            # Sprawdzamy czy dane zawierają tylko 0 i 1
            if not all(c in '01' for c in data):
                raise ValueError("Plik źródłowy może zawierać tylko znaki '0' i '1'")

            # Dzielimy dane na części
            frames = []
            for i in range(0, len(data), frame_size):
                chunk = data[i:i + frame_size]
                frame = self.create_frame(chunk)
                frames.append(frame)

            # Zapisujemy ramki do pliku
            with open(output_file, 'w') as f:
                for frame in frames:
                    f.write(frame + '\n')

            print(f"Zakodowano {len(frames)} ramek z pliku '{input_file}' do '{output_file}'")
            return True

        except Exception as e:
            print(f"Błąd podczas kodowania: {e}")
            return False

    def decode_file(self, input_file, output_file):
        """
        Dekoduje plik z ramkami do oryginalnego formatu
        :param input_file: Nazwa pliku z ramkami
        :param output_file: Nazwa pliku wyjściowego
        """
        try:
            with open(input_file, 'r') as f:
                frames = f.read().strip().split('\n')

            decoded_data = []
            errors = 0

            for i, frame in enumerate(frames):
                data, is_valid = self.extract_frame(frame)

                if is_valid:
                    decoded_data.append(data)
                else:
                    errors += 1
                    print(f"Błąd CRC w ramce {i + 1}")

            # Zapisujemy zdekodowane dane
            with open(output_file, 'w') as f:
                f.write(''.join(decoded_data))

            print(f"Zdekodowano {len(decoded_data)} ramek z '{input_file}' do '{output_file}'")
            if errors > 0:
                print(f"Wykryto {errors} błędnych ramek")
            return True

        except Exception as e:
            print(f"Błąd podczas dekodowania: {e}")
            return False


def main():
    """
    Funkcja główna - demonstracja użycia
    """
    processor = FrameProcessor()

    print("=== PROGRAM RAMKOWANIA Z BIT STUFFING I CRC ===\n")

    while True:
        print("\nWybierz opcję:")
        print("1. Zakoduj plik (Z -> W)")
        print("2. Zdekoduj plik (W -> Z)")
        print("3. Test na przykładowych danych")
        print("4. Wyjście")

        choice = input("\nTwój wybór: ")

        if choice == '1':
            input_file = input("Podaj nazwę pliku źródłowego (domyślnie 'Z'): ") or 'Z'
            output_file = input("Podaj nazwę pliku wyjściowego (domyślnie 'W'): ") or 'W'
            frame_size = input("Podaj rozmiar ramki w bitach (domyślnie 64): ")
            frame_size = int(frame_size) if frame_size else 64

            processor.encode_file(input_file, output_file, frame_size)

        elif choice == '2':
            input_file = input("Podaj nazwę pliku z ramkami (domyślnie 'W'): ") or 'W'
            output_file = input("Podaj nazwę pliku wyjściowego (domyślnie 'Z_decoded'): ") or 'Z_decoded'

            processor.decode_file(input_file, output_file)

        elif choice == '3':
            # Test na przykładowych danych
            test_data = '1011001111111010110101111110101'
            print(f"\nDane testowe: {test_data}")

            # Tworzenie ramki
            frame = processor.create_frame(test_data)
            print(f"Ramka: {frame}")

            # Dekodowanie ramki
            decoded, is_valid = processor.extract_frame(frame)
            print(f"Zdekodowane dane: {decoded}")
            print(f"CRC poprawne: {is_valid}")

            # Test bit stuffing
            stuffed = processor.bit_stuffing('111111000111111')
            print(f"\nBit stuffing dla '111111000111111': {stuffed}")
            destuffed = processor.bit_destuffing(stuffed)
            print(f"Po destuffing: {destuffed}")

        elif choice == '4':
            print("Do widzenia!")
            break
        else:
            print("Nieprawidłowy wybór!")


def example_usage():
    """
    Przykład automatycznego użycia
    """
    # Tworzenie przykładowego pliku źródłowego
    with open('Z', 'w') as f:
        f.write('10110011111110101101011111101010011100101010101111111000011')

    # Inicjalizacja procesora
    processor = FrameProcessor()

    # Kodowanie
    print("Kodowanie pliku 'Z' do 'W'...")
    processor.encode_file('Z', 'W', frame_size=20)

    # Dekodowanie
    print("\nDekodowanie pliku 'W' do 'Z_decoded'...")
    processor.decode_file('W', 'Z_decoded')

    # Sprawdzenie czy pliki są identyczne
    with open('Z', 'r') as f1, open('Z_decoded', 'r') as f2:
        if f1.read() == f2.read():
            print("\n✓ Dekodowanie przebiegło pomyślnie - pliki są identyczne!")
        else:
            print("\n✗ Błąd - pliki różnią się!")


if __name__ == "__main__":
    main()