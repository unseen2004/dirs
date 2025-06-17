class FrameProcessor:
    def __init__(self, crc_polynomial='11010101'):
        """
        Initializes the frame processor.
        :param crc_polynomial: CRC polynomial (default is CRC-8)
        """
        self.flag = '01111110'  # Frame start/end flag
        self.crc_poly = crc_polynomial
        self.stuff_bits = 5  # Insert 0 after 5 consecutive ones

    def calculate_crc(self, data):
        """
        Calculates CRC for the given data.
        :param data: Bit string (string of '0' and '1')
        :return: CRC as a bit string
        """
        # Add zeros at the end (length of polynomial - 1)
        padded_data = data + '0' * (len(self.crc_poly) - 1)

        # Convert to lists for easier manipulation
        dividend = list(padded_data)
        divisor = list(self.crc_poly)

        # Perform modulo-2 division
        for i in range(len(data)):
            if dividend[i] == '1':
                for j in range(len(divisor)):
                    dividend[i + j] = str((int(dividend[i + j]) + int(divisor[j])) % 2)

        # CRC is the remainder of the division
        remainder = ''.join(dividend[-(len(self.crc_poly) - 1):])
        return remainder

    def bit_stuffing(self, data):
        """
        Performs bit stuffing - inserts 0 after 5 consecutive ones.
        :param data: Bit string
        :return: Bit string after bit stuffing
        """
        result = []
        ones_count = 0

        for bit in data:
            result.append(bit)
            if bit == '1':
                ones_count += 1
                if ones_count == self.stuff_bits:
                    result.append('0')  # Insert 0 after 5 ones
                    ones_count = 0
            else:
                ones_count = 0

        return ''.join(result)

    def bit_destuffing(self, data):
        """
        Removes stuffed bits (destuffing).
        :param data: Bit string after bit stuffing
        :return: Original bit string
        """
        result = []
        ones_count = 0
        i = 0

        while i < len(data):
            result.append(data[i])
            if data[i] == '1':
                ones_count += 1
                if ones_count == self.stuff_bits:
                    # Skip the next 0 (it was inserted)
                    if i + 1 < len(data) and data[i + 1] == '0':
                        i += 1
                    ones_count = 0
            else:
                ones_count = 0
            i += 1

        return ''.join(result)

    def create_frame(self, data):
        """
        Creates a frame from the given data.
        :param data: Data to be placed in the frame
        :return: Complete frame
        """
        # Calculate CRC for the data
        crc = self.calculate_crc(data)

        # Combine data with CRC
        frame_content = data + crc

        # Perform bit stuffing on the frame content
        stuffed_content = self.bit_stuffing(frame_content)

        # Add start and end flags
        frame = self.flag + stuffed_content + self.flag

        return frame

    def extract_frame(self, frame):
        """
        Extracts data from the frame and verifies CRC.
        :param frame: Frame to be processed
        :return: (data, is_valid) or (None, False) if an error occurs
        """
        # Remove flags
        if not (frame.startswith(self.flag) and frame.endswith(self.flag)):
            return None, False

        content = frame[len(self.flag):-len(self.flag)]

        # Remove bit stuffing
        destuffed_content = self.bit_destuffing(content)

        # Split into data and CRC
        crc_len = len(self.crc_poly) - 1
        if len(destuffed_content) < crc_len:
            return None, False

        data = destuffed_content[:-crc_len]
        received_crc = destuffed_content[-crc_len:]

        # Verify CRC
        calculated_crc = self.calculate_crc(data)

        if calculated_crc == received_crc:
            return data, True
        else:
            return None, False

    def encode_file(self, input_file, output_file, frame_size=64):
        """
        Encodes a source file into a file with frames.
        :param input_file: Name of the input file
        :param output_file: Name of the output file
        :param frame_size: Size of the data in the frame (in bits)
        """
        try:
            with open(input_file, 'r') as f:
                data = f.read().strip()

            # Check if the data contains only 0 and 1
            if not all(c in '01' for c in data):
                raise ValueError("The source file can only contain '0' and '1' characters")

            # Split the data into chunks
            frames = []
            for i in range(0, len(data), frame_size):
                chunk = data[i:i + frame_size]
                frame = self.create_frame(chunk)
                frames.append(frame)

            # Write frames to the output file
            with open(output_file, 'w') as f:
                for frame in frames:
                    f.write(frame + '\n')

            print(f"Encoded {len(frames)} frames from file '{input_file}' to '{output_file}'")
            return True

        except Exception as e:
            print(f"Error during encoding: {e}")
            return False

    def decode_file(self, input_file, output_file):
        """
        Decodes a file with frames into the original format.
        :param input_file: Name of the file with frames
        :param output_file: Name of the output file
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
                    print(f"CRC error in frame {i + 1}")

            # Write decoded data to the output file
            with open(output_file, 'w') as f:
                f.write(''.join(decoded_data))

            print(f"Decoded {len(decoded_data)} frames from '{input_file}' to '{output_file}'")
            if errors > 0:
                print(f"Detected {errors} invalid frames")
            return True

        except Exception as e:
            print(f"Error during decoding: {e}")
            return False


def main():
    """
    Main function - demonstration of usage.
    """
    processor = FrameProcessor()

    print("=== FRAME PROCESSING PROGRAM WITH BIT STUFFING AND CRC ===\n")

    while True:
        print("\nChoose an option:")
        print("1. Encode file (Source -> Frames)")
        print("2. Decode file (Frames -> Source)")
        print("3. Test with example data")
        print("4. Exit")

        choice = input("\nYour choice: ")

        if choice == '1':
            input_file = input("Enter the name of the source file (default 'Source'): ") or 'Source'
            output_file = input("Enter the name of the output file (default 'Frames'): ") or 'Frames'
            frame_size = input("Enter the frame size in bits (default 64): ")
            frame_size = int(frame_size) if frame_size else 64

            processor.encode_file(input_file, output_file, frame_size)

        elif choice == '2':
            input_file = input("Enter the name of the file with frames (default 'Frames'): ") or 'Frames'
            output_file = input("Enter the name of the output file (default 'Source_decoded'): ") or 'Source_decoded'

            processor.decode_file(input_file, output_file)

        elif choice == '3':
            # Test with example data
            test_data = '1011001111111010110101111110101'
            print(f"\nTest data: {test_data}")

            # Create frame
            frame = processor.create_frame(test_data)
            print(f"Frame: {frame}")

            # Decode frame
            decoded, is_valid = processor.extract_frame(frame)
            print(f"Decoded data: {decoded}")
            print(f"CRC valid: {is_valid}")

            # Test bit stuffing
            stuffed = processor.bit_stuffing('111111000111111')
            print(f"\nBit stuffing for '111111000111111': {stuffed}")
            destuffed = processor.bit_destuffing(stuffed)
            print(f"After destuffing: {destuffed}")

        elif choice == '4':
            print("Goodbye!")
            break
        else:
            print("Invalid choice!")


def example_usage():
    """
    Example of automatic usage.
    """
    # Create an example source file
    with open('Source', 'w') as f:
        f.write('10110011111110101101011111101010011100101010101111111000011')

    # Initialize the processor
    processor = FrameProcessor()

    # Encoding
    print("Encoding file 'Source' to 'Frames'...")
    processor.encode_file('Source', 'Frames', frame_size=20)

    # Decoding
    print("\nDecoding file 'Frames' to 'Source_decoded'...")
    processor.decode_file('Frames', 'Source_decoded')

    # Check if files are identical
    with open('Source', 'r') as f1, open('Source_decoded', 'r') as f2:
        if f1.read() == f2.read():
            print("\n✓ Decoding successful - files are identical!")
        else:
            print("\n✗ Error - files differ!")


if __name__ == "__main__":
    main()