import matplotlib.pyplot as plt

# Dados coletados nos testes empíricos
threads = [1, 2, 4, 8]
tempo_execucao = [476.27, 237.03, 154.34, 103.57]
speedup_obtido = [1.00, 2.00, 3.09, 4.60]
speedup_ideal = [1, 2, 4, 8] 

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# Gráfico 1: Tempo de Execução vs Threads
ax1.plot(threads, tempo_execucao, marker='o', color='#1f77b4', linewidth=2, markersize=8)
ax1.set_title('Tempo de Execução vs Número de Threads', fontsize=12)
ax1.set_xlabel('Número de Threads', fontsize=10)
ax1.set_ylabel('Tempo (segundos)', fontsize=10)
ax1.set_xticks(threads)
ax1.grid(True, linestyle='--', alpha=0.6)

for i, tempo in enumerate(tempo_execucao):
    ax1.annotate(f"{tempo:.2f}s", (threads[i], tempo_execucao[i]), 
                 textcoords="offset points", xytext=(0,10), ha='center')

# Gráfico 2: Speedup Obtido vs Speedup Ideal
ax2.plot(threads, speedup_obtido, marker='s', color='#2ca02c', linewidth=2, markersize=8, label='Speedup Obtido')
ax2.plot(threads, speedup_ideal, marker='x', color='#d62728', linewidth=2, linestyle='--', label='Speedup Ideal')
ax2.set_title('Escalabilidade: Speedup Obtido vs Ideal', fontsize=12)
ax2.set_xlabel('Número de Threads', fontsize=10)
ax2.set_ylabel('Speedup (T_seq / T_par)', fontsize=10)
ax2.set_xticks(threads)
ax2.legend()
ax2.grid(True, linestyle='--', alpha=0.6)

for i, speedup in enumerate(speedup_obtido):
    ax2.annotate(f"{speedup:.2f}x", (threads[i], speedup_obtido[i]), 
                 textcoords="offset points", xytext=(0,10), ha='center')

plt.tight_layout()
plt.savefig('grafico_desempenho.png', dpi=300)
print("Gráfico gerado com sucesso e salvo como 'grafico_desempenho.png'")