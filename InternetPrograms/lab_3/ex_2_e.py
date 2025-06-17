import time
import random
import sys

DLUGOSC_KANALU = 100
MAKS_PROB = 16
CZAS_PROPAGACJI = 2

STAN_BEZCZYNNY = "bezczynny"
STAN_NADAWANIE = "nadawanie"
STAN_ZAGLUSZANIE = "zagłuszanie"
STAN_OCZEKIWANIE = "oczekiwanie"
STAN_ZAKONCZONY = "zakończony"


def inicjuj_stacje(liczba_stacji):
    stacje = []
    for i in range(liczba_stacji):
        stacja = {
            'id': chr(65 + i),  # A, B, C, ...
            'pozycja': random.randint(0, DLUGOSC_KANALU - 1),
            'czas_nadawania': random.randint(0, 10),
            'stan': STAN_BEZCZYNNY,
            'liczba_prob': 0,
            'opoznienie': 0,
            'poczatek_zagluszania': 0
        }
        stacje.append(stacja)
    return stacje


def rozchodz_sygnal(kanal, lista_sygnalow, biezacy_czas):
    nowe_sygnaly = []

    # Przesuń każdy sygnał o jedną pozycję
    for sygnal in lista_sygnalow:
        pozycja, kierunek, nadawca, czas_utworzenia = sygnal
        nowa_pozycja = pozycja + kierunek

        # Sprawdź czy sygnał nie wyszedł poza kanał
        if 0 <= nowa_pozycja < DLUGOSC_KANALU:
            # Sprawdź czy w danym miejscu jest już inny sygnał
            if kanal[nowa_pozycja] is None:
                kanal[nowa_pozycja] = nadawca
            elif kanal[nowa_pozycja] != nadawca:
                # Kolizja - oznacz jako 'X'
                kanal[nowa_pozycja] = 'X'

            nowe_sygnaly.append((nowa_pozycja, kierunek, nadawca, czas_utworzenia))

    return nowe_sygnaly


def czy_kanal_wolny(kanal, pozycja):
    return kanal[pozycja] is None


def oblicz_opoznienie(liczba_prob):
    if liczba_prob > 10:
        liczba_prob = 10  # Ograniczenie dla dużej liczby prób
    maks_wartosc = 2 ** liczba_prob - 1
    return random.randint(0, maks_wartosc) * CZAS_PROPAGACJI * DLUGOSC_KANALU


def wyswietl_kanal(kanal, pozycje_stacji):
    """Konwertuje stan kanału na czytelny string"""
    wynik = []
    for i, wartosc in enumerate(kanal):
        if i in pozycje_stacji:
            if wartosc is None:
                wynik.append(f"\033[92m{pozycje_stacji[i]}\033[0m")  # Zielony
            elif wartosc == 'X':
                wynik.append(f"\033[91m{pozycje_stacji[i]}\033[0m")  # Czerwony
            else:
                wynik.append(f"\033[93m{pozycje_stacji[i]}\033[0m")  # Żółty
        else:
            if wartosc is None:
                wynik.append('_')
            elif wartosc == 'X':
                wynik.append('\033[91mX\033[0m')  # Czerwona kolizja
            else:
                wynik.append(f"\033[94m{wartosc}\033[0m")  # Niebieski sygnał
    return ''.join(wynik)


def symuluj_csma_cd(liczba_stacji=5):
    stacje = inicjuj_stacje(liczba_stacji)
    kanal = [None] * DLUGOSC_KANALU
    sygnaly = []
    historia = []
    liczba_sukcesow = 0

    pozycje_stacji = {s['pozycja']: s['id'] for s in stacje}

    print("\n=== SYMULACJA PROTOKOŁU CSMA/CD ===")
    print(f"Długość kanału: {DLUGOSC_KANALU}")
    print(f"Liczba stacji: {liczba_stacji}")
    print("\nPozycje stacji:")
    for stacja in stacje:
        print(f"  Stacja {stacja['id']}: pozycja {stacja['pozycja']}")
    print("\nRozpoczynanie symulacji...")
    time.sleep(2)

    krok = 0
    while liczba_sukcesow < liczba_stacji and krok < 10000:
        krok += 1
        zdarzenia = []


        sygnaly = rozchodz_sygnal(kanal, sygnaly, krok)

        for stacja in stacje:
            if stacja['stan'] == STAN_BEZCZYNNY and krok >= stacja['czas_nadawania']:
                if czy_kanal_wolny(kanal, stacja['pozycja']):
                    stacja['stan'] = STAN_NADAWANIE
                    kanal[stacja['pozycja']] = stacja['id']
                    # Wyślij sygnał w obie strony
                    sygnaly.append((stacja['pozycja'], -1, stacja['id'], krok))
                    sygnaly.append((stacja['pozycja'], 1, stacja['id'], krok))
                    zdarzenia.append(f"[{stacja['id']}:START]")
                else:
                    # Kanał zajęty, czekaj
                    stacja['czas_nadawania'] = krok + 1

            elif stacja['stan'] == STAN_NADAWANIE:
                if kanal[stacja['pozycja']] == 'X':
                    stacja['stan'] = STAN_ZAGLUSZANIE
                    stacja['poczatek_zagluszania'] = krok
                    stacja['liczba_prob'] += 1
                    zdarzenia.append(f"[{stacja['id']}:KOLIZJA!]")

                    # Oblicz opóźnienie dla następnej próby
                    if stacja['liczba_prob'] < MAKS_PROB:
                        stacja['opoznienie'] = oblicz_opoznienie(stacja['liczba_prob'])
                        stacja['czas_nadawania'] = krok + CZAS_PROPAGACJI * DLUGOSC_KANALU + stacja['opoznienie']
                else:
                    kanal[stacja['pozycja']] = stacja['id']
                    sygnaly.append((stacja['pozycja'], -1, stacja['id'], krok))
                    sygnaly.append((stacja['pozycja'], 1, stacja['id'], krok))

                    # Sprawdź czy transmisja zakończona
                    if krok >= stacja['czas_nadawania'] + CZAS_PROPAGACJI * DLUGOSC_KANALU:
                        stacja['stan'] = STAN_ZAKONCZONY
                        liczba_sukcesow += 1
                        zdarzenia.append(f"[{stacja['id']}:SUKCES!]")

            # Stacja zagłuszająca
            elif stacja['stan'] == STAN_ZAGLUSZANIE:
                # Wysyłaj sygnał zagłuszający
                kanal[stacja['pozycja']] = 'X'
                sygnaly.append((stacja['pozycja'], -1, 'X', krok))
                sygnaly.append((stacja['pozycja'], 1, 'X', krok))

                # Sprawdź czy zakończyć zagłuszanie
                if krok >= stacja['poczatek_zagluszania'] + CZAS_PROPAGACJI * DLUGOSC_KANALU:
                    if stacja['liczba_prob'] < MAKS_PROB:
                        stacja['stan'] = STAN_BEZCZYNNY
                        zdarzenia.append(f"[{stacja['id']}:CZEKA({stacja['opoznienie']})]")
                    else:
                        stacja['stan'] = STAN_ZAKONCZONY
                        zdarzenia.append(f"[{stacja['id']}:REZYGNACJA]")

        # Zapisz stan kanału
        stan_kanalu = wyswietl_kanal(kanal, pozycje_stacji)
        if zdarzenia:
            historia.append(f"Krok {krok:4d}: {stan_kanalu} {' '.join(zdarzenia)}")
        else:
            historia.append(f"Krok {krok:4d}: {stan_kanalu}")

        # Wyczyść kanał dla następnego kroku
        kanal = [None] * DLUGOSC_KANALU

        # Wyświetl aktualny stan
        if krok % 10 == 0 or zdarzenia:
            print(f"\033[2J\033[H")  # Wyczyść ekran
            print("=== SYMULACJA CSMA/CD ===")
            print(f"Krok: {krok}")
            print("\nOstatnie 20 kroków:")
            for linia in historia[-20:]:
                print(linia)

            print("\nStan stacji:")
            for stacja in stacje:
                print(f"  {stacja['id']}: {stacja['stan']:12s} "
                      f"Próby: {stacja['liczba_prob']:2d} "
                      f"Następna transmisja: {stacja['czas_nadawania']:4d}")

            time.sleep(0.1)

    print("\n=== PODSUMOWANIE SYMULACJI ===")
    print(f"Całkowita liczba kroków: {krok}")
    print(f"Liczba udanych transmisji: {liczba_sukcesow}")
    for stacja in stacje:
        print(f"Stacja {stacja['id']}: {stacja['stan']} (próby: {stacja['liczba_prob']})")

    with open("symulacja_csma_cd.txt", "w", encoding="utf-8") as plik:
        plik.write("=== HISTORIA SYMULACJI CSMA/CD ===\n")
        plik.write(f"Długość kanału: {DLUGOSC_KANALU}\n")
        plik.write(f"Liczba stacji: {liczba_stacji}\n\n")
        for linia in historia:
            linia_czysta = linia.replace('\033[91m', '').replace('\033[92m', '')
            linia_czysta = linia_czysta.replace('\033[93m', '').replace('\033[94m', '')
            linia_czysta = linia_czysta.replace('\033[0m', '')
            plik.write(linia_czysta + "\n")

    print("\nHistoria zapisana do pliku: symulacja_csma_cd.txt")


if __name__ == "__main__":
    liczba_stacji = random.randint(3, 8)
    symuluj_csma_cd(liczba_stacji)