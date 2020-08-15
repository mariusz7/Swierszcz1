# Firmware for weather balloon

Firmware for AVR microcontroller measuring current temperature and sending it Morse encoded via radio channel to the ground station.

-------------------------------------------------------------------------------




Poniżej znajduje się coś na kształt dokumentacji z uwagami do programu
i prototypu elektroniki do balonu nr 1 (Świerszcz 1 :) )

Do kompilacji używam poniższych narzędzi (opis ich instalacji jest dołączony):
*binutils-2.15.92.0.2
*avr-gcc: zwykłe gcc-3.4.1 skompilowane z odpowiednimi flagami
*avr-libc-1.2.3

Osobiście miałem problemy z dwiema innymi wersjami gcc niż 3.4.1, więc jak
akurat posiadana nie będzie się chciała skompilować dla avr, to zawsze można
spróbować skompilować 3.4.1 (sprawdziłem ją pod Slackware i Gentoo).

Instalowane gcc i binutils (wg załączonej instrukcji, nie będą w żaden sposób
kolidować z już istniejącymi wersjami tych pakietów w systemie - ew. trzeba
będzie jedynie dodać do zmiennej PATH ścieżki do nowych skompilowanych
i zanistalowanych binarek)


Do "ładowania programu" do avr'a używam:
*programów: uisp lub ponyprog (ale może być inny obsługujący żądany model
 avr'a i format kodu tzw. "intel hex", który jest produkowany przy kompilacji)
*programatora: zgodnego z stk200 (dołączyłem schemat: avrisp-stk200.gif)

Dodatkowo należy mieć wkompilowaną w jądro obsługę portu równoległego i:

Device Drivers  --->  Character devices  --->
<*> Support for user-space parallel port device drivers

(albo CONFIG_PPDEV=y jak kto woli)


Jak ktoś planuje korzystać z uisp'a a wcześniej tego nie robił, to w Makefile
są gotowe cele do kasowania, ładowania i innych podstawowych rzeczy, więc nie
trzeba się będzie przebijać przez man'a.


Odpowiednie czasy trwania kropki, kreski i odstępy można ustawiać
w pliku: morse.h


Trzeba będzie ustawić tzw. Fuse bits dla avr'a, żeby poniformować go, jaki kwarc
został do niego dołączony (bo z tego co pamiętam, domyślnie ustawione jest
wewnętrzne źródło taktowania 1MHz). Można to zrobić zarówno za pomocą uisp
jak i ponyprog.


Może warto dołączyć zewnętrzny kwarc większy niż wbudowane źródło taktowania
1MHz, to atmel będzie wydzielał więcej mocy i nie da się tak łatwo wykończyć
przez niskie temperatury. Atmel i tak pobiera relatywnie mało energii, a kupiona
bateria raczej i tak wystarczy na dłużej niż nam potrzeba.


Bez fizycznego sprawdzenia/przetestowania, nie można stwierdzić, który "właśnie
kupiony" czujnik temperatury Dallas 18b20 zostanie "wykryty" jako pierwszy.
Ważne jednak, że zawsze będą wykrywane w tej samej kolejności, więc jak już raz
się sprawdzi, który odpowiada za temperature T0 a który za T1, to już tak
zostanie.


Aktualnie program jest przetestowany z dwoma czujnikami ds18b20, ale kod "jest
gotowy", do obsługi większej ich liczby (wystarczy je podłączać równolegle).
Maksymalną liczbę urządzeń, które będą wykrywane na początku ustawia się stałą
preprocesora DALLAS_MAX_DEVICES w pliku dallas.h (aktualnie 4)
Kolejne wskazywane temperatury będą miały format: T2=23,T3=26, itd.
Przykładowa sekwencja, która jest aktualnie wysyłana: "SKIK:T0=-23,T1=-34"


Testowałem czujniki w zakresie od -13 do +80 st. Celsjusza i działało :)


Zrobiłem stałą preprocesora SHOW_WARNINGS, która jeżeli zostanie zdefiniowana
w Makefile, to przy kompilacji zostaną pokazane ostrzeżenia ze wskazaniem miejsc
w kodzie, które mogą wymagać ustawienia np. po zmianie kwarcu, zmianie pinu
magistrali 1-wire itp. (w Makefile jest w komentarzu napisane, gdzie należy
wstawić definicję tej stałej). Można też ręcznie wyszukać wszystkie te miejsca
w kodzie grep'ując wszystkie pliki źródłowe "na okoliczność wystąpienia wzorca":
"warning FIXME"

Za pomocą stałej preprocesora FINAL_RELEASE, można "wyłączyć" cały kod pomocny
przy debugowaniu związany głównie z wypisywaniem na lcd komunikatów
diagnostycznych (domyślnie ta stała będzie ustawiona, czyli kod będzie gotowy
do elektroniki balonu, który oczywiście nie będzie miał żadnego lcd'ka,
bo i kto miałby na niego spoglądać tam na górze :-) )


"Biblioteka" do obsługi 1-wire jest do pobrania ze strony producenta tego
termometru, więc nie musiałem wszystkiego sam robić, tylko dostosowałem
to do avr-libc, bo te oryginalne pliki były początkowo nie do skompilowania
przez avr-gcc :) (chodzi o pliki: dallas.h dallas.c ds18b20.h ds18b20.c
dallasconf.h)


Jak się tak zastanowić, to się okazuje, że schemat elektroniki będzie całkiem
prosty (i taki miał chyba być dla pierwszego balona):
*Atmel ATMega8 (lub wyższy - ja program testowałem na ATMega16, ale powinien
 być przenośny),
*zewnętrzny kwarc i 2 kondensatry (teoretycznie opcjonalne, bo można wykorzystać
 wewnętrzny układ taktujący 1MHz, ale może jednak warto zostawić ze względów
 opisanych wcześniej)
*2 czujniki ds18b20 podłączone linią danych równolegle do jakiegoś wolnego pinu
 IO avr'a, czyli do prawie dowolnego :)
*układ nadawczy z jednym wejściem na sygnał kluczujący z avr'a, czyli drugi pin
 IO avr'a do wykorzystania - a pozostałe będą wolne

Schemat układu można znaleźć w pliku: schemat.png

Może się wydawać, że można było spokojnie "zadowolić się" prostszym układem -
i jest to racja, tylko wydaje mi się, że kolejne ew. elektroniki do balonów
nie będą już tak proste, i programy też będą bardziej skomplikowane. Po tym
balonie już będzie przetestowana "platforma" pod przyszłe rozwiązania -
zobaczy się jak ten avr sobie poradzi w niskich temperaturach.

Jeżeli wybrany avr to nie będzie ATMega16 (a prawdopodobnie nie będzie),
to trzeba poprawić stałe w Makefile, żeby uwzględnić ten fakt.

W sumie, jak elektronika prototypowa będzie gotowa, to sam mogę (po otrzymaniu
potrzebnych informacji) dostosować program, przez modyfikacje różnych stałych,
tak żeby nikt już nie musiał instalować specjalnie gcc, binutils i avr-libc.
Podeślę tylko plik w formacie "intel hex" gotowy do załadowania do atmela.


Wykorzystywany czujnik ds18b20 nie tylko wskazuje aktualną temperaturę, jeżeli
jest ona z obsługiwanego zakresu od -55 do +125 st. Celsjusza, ale też wskazuje,
że jest ona poza zakresem (jeżeli faktycznie jest). Jeżeli temp. spadła poniżej
-55 st. to odczytywana wartość będzie równa zawsze -56 a jeżeli powyżej +125,
to zawsze odczytamy +126 - no chyba że czujnik się stopi :) (są to po prostu
wartości specjalne, które mimo że wyglądają na wiarygodne/prawdopodobne, nie
mogą być brane dosłownie).

Aktualnie wysyłana temperatura jest jako liczba całkowita. Wykorzystywane
czujniki umożliwiają jednak pomiar również części ułamkowych. W aktualnym
programie nie są wykorzystywane, bo przy zbliżaniu się do granicy zakresu
pomiarowego błąd tegoż pomiaru wzrasta do +/- 2 st. Celsjusza, to odpuściłem
sobie jakies tam ułamki (dokładność +/- 0.5 st. jest tylko w zakresie od -10
do +85 st.). Jeżeli jednak się mylę i warto je wykorzystać to proszę o info.


Wykorzystywany czujnik może "obejść się" bez zwykłej linii zasilającej,
ale ja bym ją dał żeby nie kombinować. Nawet nie testowałem tych 2 czujników
w takim układzie. W samej dokumentacji znalazłem wzmiankę, że powyżej 100 st.
lepiej używać normalnego zasilania. U nas będą temperatury z drugiego końca
zakresu, ale kto wie czy też nie wystąpują wtedy jakieś problemy - po co
ryzykować.
