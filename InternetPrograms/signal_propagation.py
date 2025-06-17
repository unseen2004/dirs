import time
import random
import sys

CHANNEL_LENGTH = 100
MAX_ATTEMPTS = 16
PROPAGATION_TIME = 2

STATE_IDLE = "idle"
STATE_TRANSMITTING = "transmitting"
STATE_JAMMING = "jamming"
STATE_WAITING = "waiting"
STATE_COMPLETED = "completed"


def initialize_stations(num_stations):
    stations = []
    for i in range(num_stations):
        station = {
            'id': chr(65 + i),  # A, B, C, ...
            'position': random.randint(0, CHANNEL_LENGTH - 1),
            'transmission_time': random.randint(0, 10),
            'state': STATE_IDLE,
            'attempts': 0,
            'delay': 0,
            'jamming_start': 0
        }
        stations.append(station)
    return stations


def propagate_signal(channel, signal_list, current_time):
    new_signals = []

    # Move each signal by one position
    for signal in signal_list:
        position, direction, sender, creation_time = signal
        new_position = position + direction

        # Check if the signal is within the channel bounds
        if 0 <= new_position < CHANNEL_LENGTH:
            # Check if the position is already occupied
            if channel[new_position] is None:
                channel[new_position] = sender
            elif channel[new_position] != sender:
                # Collision - mark as 'X'
                channel[new_position] = 'X'

            new_signals.append((new_position, direction, sender, creation_time))

    return new_signals


def is_channel_free(channel, position):
    return channel[position] is None


def calculate_delay(attempts):
    if attempts > 10:
        attempts = 10  # Limit for large number of attempts
    max_value = 2 ** attempts - 1
    return random.randint(0, max_value) * PROPAGATION_TIME * CHANNEL_LENGTH


def display_channel(channel, station_positions):
    """Converts the channel state to a readable string"""
    result = []
    for i, value in enumerate(channel):
        if i in station_positions:
            if value is None:
                result.append(f"\033[92m{station_positions[i]}\033[0m")  # Green
            elif value == 'X':
                result.append(f"\033[91m{station_positions[i]}\033[0m")  # Red
            else:
                result.append(f"\033[93m{station_positions[i]}\033[0m")  # Yellow
        else:
            if value is None:
                result.append('_')
            elif value == 'X':
                result.append('\033[91mX\033[0m')  # Red collision
            else:
                result.append(f"\033[94m{value}\033[0m")  # Blue signal
    return ''.join(result)


def simulate_csma_cd(num_stations=5):
    stations = initialize_stations(num_stations)
    channel = [None] * CHANNEL_LENGTH
    signals = []
    history = []
    successful_transmissions = 0

    station_positions = {s['position']: s['id'] for s in stations}

    print("\n=== CSMA/CD PROTOCOL SIMULATION ===")
    print(f"Channel length: {CHANNEL_LENGTH}")
    print(f"Number of stations: {num_stations}")
    print("\nStation positions:")
    for station in stations:
        print(f"  Station {station['id']}: position {station['position']}")
    print("\nStarting simulation...")
    time.sleep(2)

    step = 0
    while successful_transmissions < num_stations and step < 10000:
        step += 1
        events = []

        signals = propagate_signal(channel, signals, step)

        for station in stations:
            if station['state'] == STATE_IDLE and step >= station['transmission_time']:
                if is_channel_free(channel, station['position']):
                    station['state'] = STATE_TRANSMITTING
                    channel[station['position']] = station['id']
                    # Send signal in both directions
                    signals.append((station['position'], -1, station['id'], step))
                    signals.append((station['position'], 1, station['id'], step))
                    events.append(f"[{station['id']}:START]")
                else:
                    # Channel busy, wait
                    station['transmission_time'] = step + 1

            elif station['state'] == STATE_TRANSMITTING:
                if channel[station['position']] == 'X':
                    station['state'] = STATE_JAMMING
                    station['jamming_start'] = step
                    station['attempts'] += 1
                    events.append(f"[{station['id']}:COLLISION!]")

                    # Calculate delay for the next attempt
                    if station['attempts'] < MAX_ATTEMPTS:
                        station['delay'] = calculate_delay(station['attempts'])
                        station['transmission_time'] = step + PROPAGATION_TIME * CHANNEL_LENGTH + station['delay']
                else:
                    channel[station['position']] = station['id']
                    signals.append((station['position'], -1, station['id'], step))
                    signals.append((station['position'], 1, station['id'], step))

                    # Check if transmission is completed
                    if step >= station['transmission_time'] + PROPAGATION_TIME * CHANNEL_LENGTH:
                        station['state'] = STATE_COMPLETED
                        successful_transmissions += 1
                        events.append(f"[{station['id']}:SUCCESS!]")

            # Jamming station
            elif station['state'] == STATE_JAMMING:
                # Send jamming signal
                channel[station['position']] = 'X'
                signals.append((station['position'], -1, 'X', step))
                signals.append((station['position'], 1, 'X', step))

                # Check if jamming should end
                if step >= station['jamming_start'] + PROPAGATION_TIME * CHANNEL_LENGTH:
                    if station['attempts'] < MAX_ATTEMPTS:
                        station['state'] = STATE_IDLE
                        events.append(f"[{station['id']}:WAIT({station['delay']})]")
                    else:
                        station['state'] = STATE_COMPLETED
                        events.append(f"[{station['id']}:GIVE UP]")

        # Save channel state
        channel_state = display_channel(channel, station_positions)
        if events:
            history.append(f"Step {step:4d}: {channel_state} {' '.join(events)}")
        else:
            history.append(f"Step {step:4d}: {channel_state}")

        # Clear channel for the next step
        channel = [None] * CHANNEL_LENGTH

        # Display current state
        if step % 10 == 0 or events:
            print(f"\033[2J\033[H")  # Clear screen
            print("=== CSMA/CD SIMULATION ===")
            print(f"Step: {step}")
            print("\nLast 20 steps:")
            for line in history[-20:]:
                print(line)

            print("\nStation states:")
            for station in stations:
                print(f"  {station['id']}: {station['state']:12s} "
                      f"Attempts: {station['attempts']:2d} "
                      f"Next transmission: {station['transmission_time']:4d}")

            time.sleep(0.1)

    print("\n=== SIMULATION SUMMARY ===")
    print(f"Total steps: {step}")
    print(f"Number of successful transmissions: {successful_transmissions}")
    for station in stations:
        print(f"Station {station['id']}: {station['state']} (attempts: {station['attempts']})")

    with open("csma_cd_simulation.txt", "w", encoding="utf-8") as file:
        file.write("=== CSMA/CD SIMULATION HISTORY ===\n")
        file.write(f"Channel length: {CHANNEL_LENGTH}\n")
        file.write(f"Number of stations: {num_stations}\n\n")
        for line in history:
            clean_line = line.replace('\033[91m', '').replace('\033[92m', '')
            clean_line = clean_line.replace('\033[93m', '').replace('\033[94m', '')
            clean_line = clean_line.replace('\033[0m', '')
            file.write(clean_line + "\n")

    print("\nHistory saved to file: csma_cd_simulation.txt")


if __name__ == "__main__":
    num_stations = random.randint(3, 8)
    simulate_csma_cd(num_stations)