import matplotlib.pyplot as plt

# データ
capacitance_pf = [0, 100, 220, 330, 470, 680]
time_us = [2.3, 5.8, 10, 12, 16, 25]

# グラフ作成
plt.figure()
plt.plot(capacitance_pf, time_us, marker='o')
plt.xlabel('Capacitance(pF)')
plt.ylabel('Charging Time(μs)')
plt.grid(True)
plt.tight_layout()
plt.savefig('graph.png', dpi=300)
plt.show()
