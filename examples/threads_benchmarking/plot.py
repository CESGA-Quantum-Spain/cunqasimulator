import matplotlib.pyplot as plt

X = range(7, 36)
H_no_threads = [1813, 402, 500, 835, 1565, 2894, 5545, 10947, 29448, 49825, 96888, 685368, 1684726, 4644433, 10526296, 22388147, 46015497, 93174827, 187395074, 375888855, 752778127, 1506814940, 3015090563, 6038403609, 12074106025, 24146853372, 48291405808, 96602846630, 187115096060]
H_threads = [2468554, 1810410, 1598689, 1784239, 1556250, 1730414, 1502956, 1688214, 1466584, 1705739, 1523990, 1660069, 1713533, 1850598, 2028495, 2771500, 3173305, 5543894, 9220288, 19109322, 40199573, 79792057, 146753107, 297877198, 591210999, 1188403944, 2356466469, 4691703695, 9105901992]

CX_no_threads = [736, 275, 313, 500, 829, 1534, 3000, 6468, 12801, 25756, 50043, 103694, 243693, 468085, 990257, 2448783, 5890156, 11944446, 24258817, 48919409, 97405942, 194225695, 391028464, 782162797, 1566643570, 3140127577, 6273716757, 12549574623, 25382293222]
CX_threads = [1953130, 1540348, 1723958, 1466068, 1654426, 1523273, 1602115, 1474613, 1580692, 1464022, 1595370, 1440414, 1660085, 1434937, 1613182, 1705412, 2437981, 3039026, 5850999, 10350728, 19782371, 37975578, 73034357, 146438155, 290885945, 584665897, 1181093188, 2358188648, 4515113319]

Measure_no_threads = [15192, 8642, 10252, 11215, 17716, 28623, 56867, 71186, 160032, 310840, 752547, 1481320, 3495388, 6676595, 14845736, 29986020, 63723496, 132562864, 269178533, 545595408, 1097916831, 2204095957, 4418949274, 8941132081, 17885231196, 35803320791, 71585598484, 143217419848, 399220165074]
Measure_threads = [2128168, 2078651, 1875597, 2005007, 1793884, 1949069, 1750098, 1931213, 1706997, 1882004, 1761538, 1934169, 1928953, 2420173, 2557676, 3232196, 4491964, 7243282, 13360637, 26137376, 52506258, 104961317, 203839654, 409645868, 826057340, 1653176979, 3319990113, 6569578464, 12802908907]

Aer_H = [1840310, 36068, 27061, 28381, 32708, 41412, 59710, 100573, 174446, 327910, 619485, 1243630, 3267284, 7583938, 17676083, 34819735, 70520215, 140045582, 279913896, 561249197, 1121867550, 2240323224, 4482191206, 8979952686, 17977337801, 35961563453, 71865189815, 144086241342, 280418047695]
Aer_CX = [336209, 24715, 23479, 25236, 28857, 40090, 54735, 87571, 154188, 290514, 547884, 1075112, 2125053, 4197472, 8465914, 16891718, 33998353, 67733756, 135322824, 270838540, 542050362, 1084357327, 2180919302, 4358061230, 8722719650, 17438023224, 34902316295, 69795453830, 139932279654]
Aer_Measure = [626279, 12980, 14668, 22415, 37270, 64128, 127273, 234309, 488962, 953156, 1910078, 3783924, 8507816, 18207874, 39015982, 79054039, 157652749, 316173932, 633070913, 1284795903, 2539388486, 5072887211, 10127185104, 20332436116, 40662015548, 81324818910, 164920602350, 325255955681, 652014174032]


################### H ################
fig_h, axes_h = plt.subplots(1, 2, figsize=(12, 5))
axes_h[0].plot(X, H_no_threads, color='blue', label="Cunqa No Threads")
axes_h[0].plot(X, H_threads, color='red', label="Cunqa No Threads")
axes_h[0].plot(X, Aer_H, color='green', label="Aer")
axes_h[0].set_xlabel('N_Qubits')
axes_h[0].set_ylabel('Time (ns)')
axes_h[0].grid(True)
axes_h[0].set_title('Usual scale')
axes_h[0].legend()

axes_h[1].plot(X, H_no_threads, color='blue', label="Cunqa No Threads")
axes_h[1].plot(X, H_threads, color='red', label="Cunqa No Threads")
axes_h[1].plot(X, Aer_H, color='green', label="Aer")
axes_h[1].set_xlabel('N_Qubits')
axes_h[1].set_ylabel('Time (ns)')
axes_h[1].set_yscale('log')
axes_h[1].grid(True)
axes_h[1].set_title('Logarithmic scale')
axes_h[1].legend()

plt.tight_layout()
plt.savefig("H_Gate.png", dpi=300)


################### CX ################
fig_cx, axes_cx = plt.subplots(1, 2, figsize=(12, 5))
axes_cx[0].plot(X, CX_no_threads, color='blue', label="Cunqa No Threads")
axes_cx[0].plot(X, CX_threads, color='red', label="Cunqa No Threads")
axes_cx[0].plot(X, Aer_CX, color='green', label="Aer")
axes_cx[0].set_xlabel('N_Qubits')
axes_cx[0].set_ylabel('Time (ns)')
axes_cx[0].grid(True)
axes_cx[0].set_title('Usual scale')
axes_cx[0].legend()

axes_cx[1].plot(X, CX_no_threads, color='blue', label="Cunqa No Threads")
axes_cx[1].plot(X, CX_threads, color='red', label="Cunqa No Threads")
axes_cx[1].plot(X, Aer_CX, color='green', label="Aer")
axes_cx[1].set_xlabel('N_Qubits')
axes_cx[1].set_ylabel('Time (ns)')
axes_cx[1].set_yscale('log')
axes_cx[1].grid(True)
axes_cx[1].set_title('Logarithmic scale')
axes_cx[1].legend()

plt.tight_layout()
plt.savefig("CX_Gate.png", dpi=300)


################### MEASURE ################
fig_measure, axes_measure = plt.subplots(1, 2, figsize=(12, 5))
axes_measure[0].plot(X, Measure_no_threads, color='blue', label="Cunqa No Threads")
axes_measure[0].plot(X, Measure_threads, color='red', label="Cunqa No Threads")
axes_measure[0].plot(X, Aer_Measure, color='green', label="Aer")
axes_measure[0].set_xlabel('N_Qubits')
axes_measure[0].set_ylabel('Time (ns)')
axes_measure[0].grid(True)
axes_measure[0].set_title('Usual scale')
axes_measure[0].legend()

axes_measure[1].plot(X, Measure_no_threads, color='blue', label="Cunqa No Threads")
axes_measure[1].plot(X, Measure_threads, color='red', label="Cunqa No Threads")
axes_measure[1].plot(X, Aer_Measure, color='green', label="Aer")
axes_measure[1].set_xlabel('N_Qubits')
axes_measure[1].set_ylabel('Time (ns)')
axes_measure[1].set_yscale('log')
axes_measure[1].grid(True)
axes_measure[1].set_title('Logarithmic scale')
axes_measure[1].legend()

plt.tight_layout()
plt.savefig("Measure.png", dpi=300)
