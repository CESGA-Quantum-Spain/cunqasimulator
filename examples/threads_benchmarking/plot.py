import matplotlib.pyplot as plt

X = range(7, 36)
H_no_threads = [1813, 402, 500, 835, 1565, 2894, 5545, 10947, 29448, 49825, 96888, 685368, 1684726, 4644433, 10526296, 22388147, 46015497, 93174827, 187395074, 375888855, 752778127, 1506814940, 3015090563, 6038403609, 12074106025, 24146853372, 48291405808, 96602846630, 187115096060]
H_16_threads = [742077, 287620, 260069, 246865, 253833, 248902, 247328, 248736, 253387, 267902, 275963, 297655, 354346, 601453, 1001985, 1808210, 3337694, 6339741, 12387138, 24480332, 48797060, 96955091, 193175326, 390995228, 778913197, 1558920736, 3115098301, 6241300000, 12426802848]
H_32_threads = [1093005, 648026, 602300, 613926, 627321, 595097, 600923, 593161, 640191, 607640, 617133, 629387, 667644, 747026, 1080269, 1497771, 2418237, 4615858, 9515360, 18965102, 39157628, 76895837, 152213771, 305300815, 603167797, 1195755713, 2386203423, 4770411417, 9343693998]
H_64_threads = [2468554, 1810410, 1598689, 1784239, 1556250, 1730414, 1502956, 1688214, 1466584, 1705739, 1523990, 1660069, 1713533, 1850598, 2028495, 2771500, 3173305, 5543894, 9220288, 19109322, 40199573, 79792057, 146753107, 297877198, 591210999, 1188403944, 2356466469, 4691703695, 9105901992]
H_128_threads = [0, 3713979, 3160949, 3054173, 3038628, 3013949, 3066184, 2969982, 3064023, 3031740, 3113232, 3201130, 3350558, 3352420, 3701069, 4210733, 4794849, 6128145, 9688184, 19002506, 35943434, 70888109, 141458897, 282032561, 555717930, 1126245220, 2239027724, 4447326542, 8717601596]
H_256_threads = [0, 0, 6613310, 5925177, 5870259, 5800136, 5775482, 5903898, 5718393, 5977656, 5868644, 5992146, 6138482, 6482864, 6060989, 6753735, 7860300, 10038299, 11473828, 18622119, 37922823, 71963133, 140758800, 277902105, 554959989, 1109766741, 2210311157, 4431875623, 8663975925]


CX_no_threads = [736, 275, 313, 500, 829, 1534, 3000, 6468, 12801, 25756, 50043, 103694, 243693, 468085, 990257, 2448783, 5890156, 11944446, 24258817, 48919409, 97405942, 194225695, 391028464, 782162797, 1566643570, 3140127577, 6273716757, 12549574623, 25382293222]
CX_32_threads = [603860, 600554, 638137, 581117, 587222, 564089, 573128, 561282, 557653, 576270, 545595, 563510, 576839, 622094, 747721, 963401, 1333643, 2580839, 5195309, 10275958, 20606968, 40003316, 80600714, 162001857, 320518991, 639702075, 1281637200, 2565049712, 5055980211]
CX_16_threads = [274798, 272134, 257689, 263937, 262296, 244819, 242995, 243292, 241480, 259180, 248995, 256331, 334387, 374544, 500402, 769809, 1469277, 3240372, 6865262, 13789545, 27931976, 55269042, 110489171, 220209378, 441877753, 860807200, 1709672241, 3404046526, 6558716262]
CX_64_threads = [1953130, 1540348, 1723958, 1466068, 1654426, 1523273, 1602115, 1474613, 1580692, 1464022, 1595370, 1440414, 1660085, 1434937, 1613182, 1705412, 2437981, 3039026, 5850999, 10350728, 19782371, 37975578, 73034357, 146438155, 290885945, 584665897, 1181093188, 2358188648, 4515113319]
CX_128_threads = [0, 3319955, 2975102, 3000099, 2991316, 2908262, 3007783, 2978024, 3007800, 2964570, 2962358, 2993964, 3056560, 3081798, 3112803, 3049885, 3833213, 4727554, 6543028, 11160089, 20736566, 38389405, 77933796, 152758410, 305453270, 601466407, 1170959146, 2354286331, 4558925229]
CX_256_threads = [0, 0, 6090130, 5812596, 5708622, 5715229, 5694076, 5733643, 5674412, 5818767, 5689778, 5828416, 5807088, 5958050, 6130680, 5991396, 7116997, 8148508, 8818036, 12815094, 23480257, 41121159, 73326805, 144906995, 286488313, 568116592, 1137965919, 2239132850, 4310982232]

Measure_no_threads = [15192, 8642, 10252, 11215, 17716, 28623, 56867, 71186, 160032, 310840, 752547, 1481320, 3495388, 6676595, 14845736, 29986020, 63723496, 132562864, 269178533, 545595408, 1097916831, 2204095957, 4418949274, 8941132081, 17885231196, 35803320791, 71585598484, 143217419848, 399220165074]
Measure_16_threads = [558530, 322038, 310863, 317818, 320126, 324664, 314598, 312806, 319135, 330590, 353531, 392245, 681341, 1018615, 1230991, 2011449, 4225338, 8980211, 18233205, 36412186, 72527449, 144603007, 288729963, 578899056, 1152118918, 2310203563, 4607235470, 9254826093, 18181890972]
Measure_32_threads = [920999, 747587, 747291, 730621, 730864, 708113, 703832, 698279, 700503, 729278, 794505, 754149, 795326, 953208, 1594978, 2391185, 3002873, 6556963, 13847505, 27717143, 54979322, 109746872, 219606447, 431991265, 871902520, 1741560664, 3445687963, 6952343236, 13578923298]
Measure_64_threads = [2128168, 2078651, 1875597, 2005007, 1793884, 1949069, 1750098, 1931213, 1706997, 1882004, 1761538, 1934169, 1928953, 2420173, 2557676, 3232196, 4491964, 7243282, 13360637, 26137376, 52506258, 104961317, 203839654, 409645868, 826057340, 1653176979, 3319990113, 6569578464, 12802908907]
Measure_128_threads = [0, 4091958, 3685246, 3683150, 3617465, 3418827, 3519889, 3468251, 3344881, 3377424, 3406801, 3464331, 3467502, 3809260, 4451675, 4730432, 5753516, 8535962, 14314388, 27900003, 51345638, 106488579, 219564053, 427164775, 815729912, 1706964848, 3439305471, 6670791444, 13053909983]
Measure_256_threads = [0, 0, 8064111, 7122077, 7012610, 6953498, 6759308, 6660926, 6736562, 6841268, 6849524, 6778201, 6869445, 7388304, 7629843, 8539890, 9784403, 12340597, 17650437, 26751550, 63910383, 117080796, 217584820, 441654029, 878189506, 1794752346, 3503349083, 7152625223, 13161143263]

Aer_H = [1840310, 36068, 27061, 28381, 32708, 41412, 59710, 100573, 174446, 327910, 619485, 1243630, 3267284, 7583938, 17676083, 34819735, 70520215, 140045582, 279913896, 561249197, 1121867550, 2240323224, 4482191206, 8979952686, 17977337801, 35961563453, 71865189815, 144086241342, 280418047695]
Aer_CX = [336209, 24715, 23479, 25236, 28857, 40090, 54735, 87571, 154188, 290514, 547884, 1075112, 2125053, 4197472, 8465914, 16891718, 33998353, 67733756, 135322824, 270838540, 542050362, 1084357327, 2180919302, 4358061230, 8722719650, 17438023224, 34902316295, 69795453830, 139932279654]
Aer_Measure = [626279, 12980, 14668, 22415, 37270, 64128, 127273, 234309, 488962, 953156, 1910078, 3783924, 8507816, 18207874, 39015982, 79054039, 157652749, 316173932, 633070913, 1284795903, 2539388486, 5072887211, 10127185104, 20332436116, 40662015548, 81324818910, 164920602350, 325255955681, 652014174032]


################### H ################
fig_h, axes_h = plt.subplots(1, 2, figsize=(12, 5))
axes_h[0].plot(X, H_no_threads, color='blue', label="Cunqa No Threads")
axes_h[0].plot(X, H_16_threads, color='red', label="Cunqa 16 Threads")
axes_h[0].plot(X, Aer_H, color='green', label="Aer")
axes_h[0].set_xlabel('N_Qubits')
axes_h[0].set_ylabel('Time (ns)')
axes_h[0].grid(True)
axes_h[0].set_title('Usual scale')
axes_h[0].legend()

axes_h[1].plot(X, H_no_threads, color='blue', label="Cunqa No Threads")
axes_h[1].plot(X, H_16_threads, color='red', label="Cunqa 16 Threads")
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
fig_cx, axes_cx = plt.subplots(1, 2, figsize=(20, 10))
axes_cx[0].plot(X, CX_no_threads, color='blue', label="Cunqa No Threads")
axes_cx[0].plot(X, CX_16_threads, color='red', label="Cunqa 16 Threads")
axes_cx[0].plot(X, Aer_CX, color='green', label="Aer")
axes_cx[0].set_xlabel('N_Qubits')
axes_cx[0].set_ylabel('Time (ns)')
axes_cx[0].grid(True)
axes_cx[0].set_title('Usual scale')
axes_cx[0].legend()

axes_cx[1].plot(X, CX_no_threads, color='blue', label="Cunqa No Threads")
axes_cx[1].plot(X, CX_32_threads, color='red', label="Cunqa 32 Threads")
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
axes_measure[0].plot(X, Measure_16_threads, color='red', label="Cunqa 16 Threads")
axes_measure[0].plot(X, Aer_Measure, color='green', label="Aer")
axes_measure[0].set_xlabel('N_Qubits')
axes_measure[0].set_ylabel('Time (ns)')
axes_measure[0].grid(True)
axes_measure[0].set_title('Usual scale')
axes_measure[0].legend()

axes_measure[1].plot(X, Measure_no_threads, color='blue', label="Cunqa No Threads")
axes_measure[1].plot(X, Measure_16_threads, color='red', label="Cunqa 16 Threads")
axes_measure[1].plot(X, Aer_Measure, color='green', label="Aer")
axes_measure[1].set_xlabel('N_Qubits')
axes_measure[1].set_ylabel('Time (ns)')
axes_measure[1].set_yscale('log')
axes_measure[1].grid(True)
axes_measure[1].set_title('Logarithmic scale')
axes_measure[1].legend()

plt.tight_layout()
plt.savefig("Measure.png", dpi=300)



""" 
################## Number of threads comparison ########

fig_threads, axes_threads = plt.subplots(2, 3, figsize=(12, 5))
axes_threads[0, 0].plot(X[-10:], H_16_threads[-10:], color='black', label="16 Threads")
axes_threads[0, 0].plot(X[-10:], H_32_threads[-10:], color='red', label="32 Threads")
axes_threads[0, 0].plot(X[-10:], H_64_threads[-10:], color='blue', label="64 Threads")
axes_threads[0, 0].plot(X[-10:], H_128_threads[-10:], color='green', label="128 Threads")
axes_threads[0, 0].plot(X[-10:], H_256_threads[-10:], color='yellow', label="256 Threads")
axes_threads[0, 0].set_xlabel('N_Qubits')
axes_threads[0, 0].set_ylabel('Time (ns)')
axes_threads[0, 0].grid(True)
axes_threads[0, 0].set_title('H Gate')
axes_threads[0, 0].legend()

axes_threads[0, 1].plot(X[-10:], CX_16_threads[-10:], color='black', label="16 Threads")
axes_threads[0, 1].plot(X[-10:], CX_32_threads[-10:], color='red', label="32 Threads")
axes_threads[0, 1].plot(X[-10:], CX_64_threads[-10:], color='blue', label="64 Threads")
axes_threads[0, 1].plot(X[-10:], CX_128_threads[-10:], color='green', label="128 Threads")
axes_threads[0, 1].plot(X[-10:], CX_256_threads[-10:], color='yellow', label="256 Threads")
axes_threads[0, 1].set_xlabel('N_Qubits')
axes_threads[0, 1].set_ylabel('Time (ns)')
axes_threads[0, 1].grid(True)
axes_threads[0, 1].set_title('CX Gate')
axes_threads[0, 1].legend()

axes_threads[0, 2].plot(X[-10:], Measure_16_threads[-10:], color='black', label="16 Threads")
axes_threads[0, 2].plot(X[-10:], Measure_32_threads[-10:], color='red', label="32 Threads")
axes_threads[0, 2].plot(X[-10:], Measure_64_threads[-10:], color='blue', label="64 Threads")
axes_threads[0, 2].plot(X[-10:], Measure_128_threads[-10:], color='green', label="128 Threads")
axes_threads[0, 2].plot(X[-10:], Measure_256_threads[-10:], color='yellow', label="256 Threads")
axes_threads[0, 2].set_xlabel('N_Qubits')
axes_threads[0, 2].set_ylabel('Time (ns)')
axes_threads[0, 2].grid(True)
axes_threads[0, 2].set_title('Measure')
axes_threads[0, 2].legend()

axes_threads[1, 0].plot(X[-10:], H_16_threads[-10:], color='black', label="16 Threads")
axes_threads[1, 0].plot(X[-10:], H_32_threads[-10:], color='red', label="32 Threads")
axes_threads[1, 0].plot(X[-10:], H_64_threads[-10:], color='blue', label="64 Threads")
axes_threads[1, 0].plot(X[-10:], H_128_threads[-10:], color='green', label="128 Threads")
axes_threads[1, 0].plot(X[-10:], H_256_threads[-10:], color='yellow', label="256 Threads")
axes_threads[1, 0].set_xlabel('N_Qubits')
axes_threads[1, 0].set_ylabel('Time (ns)')
axes_threads[1, 0].set_yscale('log')
axes_threads[1, 0].grid(True)
axes_threads[1, 0].set_title('H Gate')
axes_threads[1, 0].legend()

axes_threads[1, 1].plot(X[-10:], CX_16_threads[-10:], color='black', label="16 Threads")
axes_threads[1, 1].plot(X[-10:], CX_32_threads[-10:], color='red', label="32 Threads")
axes_threads[1, 1].plot(X[-10:], CX_64_threads[-10:], color='blue', label="64 Threads")
axes_threads[1, 1].plot(X[-10:], CX_128_threads[-10:], color='green', label="128 Threads")
axes_threads[1, 1].plot(X[-10:], CX_256_threads[-10:], color='yellow', label="256 Threads")
axes_threads[1, 1].set_xlabel('N_Qubits')
axes_threads[1, 1].set_ylabel('Time (ns)')
axes_threads[1, 1].set_yscale('log')
axes_threads[1, 1].grid(True)
axes_threads[1, 1].set_title('CX Gate')
axes_threads[1, 1].legend()

axes_threads[1, 2].plot(X[-10:], Measure_16_threads[-10:], color='black', label="16 Threads")
axes_threads[1, 2].plot(X[-10:], Measure_32_threads[-10:], color='red', label="32 Threads")
axes_threads[1, 2].plot(X[-10:], Measure_64_threads[-10:], color='blue', label="64 Threads")
axes_threads[1, 2].plot(X[-10:], Measure_128_threads[-10:], color='green', label="128 Threads")
axes_threads[1, 2].plot(X[-10:], Measure_256_threads[-10:], color='yellow', label="256 Threads")
axes_threads[1, 2].set_xlabel('N_Qubits')
axes_threads[1, 2].set_ylabel('Time (ns)')
axes_threads[1, 2].set_yscale('log')
axes_threads[1, 2].grid(True)
axes_threads[1, 2].set_title('Measure')
axes_threads[1, 2].legend()


plt.tight_layout()
plt.savefig("Threads_26-35.png", dpi=300) """
