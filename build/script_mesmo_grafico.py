import os
import numpy as np
import matplotlib.pyplot as plt
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import letter

h = 4.135667696e-15  # Constante de Planck (eV*s)
c = 3e8  # Velocidade da luz (m/s)

GeV_to_eV = 1e9  # Conversão de GeV para eV

def calculate_wavelength(energy_gev):
    if energy_gev <= 0:  # Energia inválida
        return None
    energy_ev = energy_gev * GeV_to_eV  # Converter de GeV para eV
    wavelength_m = (h * c) / energy_ev  # Comprimento de onda em metros
    wavelength_nm = wavelength_m * 1e9  # Convertendo para nanômetros
    return wavelength_nm

energias_iniciais = ["0.5", "1", "5", "10"]
elementos = ["AIR", "CO2", "CH4"]
cores = {
    "AIR": "blue",
    "CO2": "green",
    "CH4": "red"
}

image_files = []
mean_wavelengths = {elemento: [] for elemento in elementos}

# Primeiro vamos coletar todos os dados
dados_por_energia = {e: {elem: [] for elem in elementos} for e in energias_iniciais}

for elemento in elementos:
    for e_inicial in energias_iniciais:
        e = e_inicial.replace(".", "")
        output_file_name = f"{elemento}_E0_{e}_n_100"
        input_file = output_file_name + ".hit"

        if not os.path.exists(input_file):
            print(f"Arquivo {input_file} não encontrado. Pulando...")
            continue

        gamma_energies = []

        # Lendo o arquivo para capturar energias cinéticas de partículas gama
        with open(input_file, "r") as file:
            for line in file:
                data = line.strip().split()
                particle = data[11]  # Identificando a partícula
                e_kin_gev = float(data[10])  # Energia cinética (GeV)
                if particle == "gamma" and e_kin_gev > 0:  # Apenas gammas com energia válida
                    gamma_energies.append(e_kin_gev)

        # Calculando comprimentos de onda
        wavelengths = [calculate_wavelength(e) for e in gamma_energies if calculate_wavelength(e) is not None]

        if not wavelengths:  # Ignorar arquivos sem dados relevantes
            print(f"Nenhum dado relevante encontrado no arquivo {input_file}. Pulando...")
            continue

        # Armazenando os dados para o histograma agrupado
        dados_por_energia[e_inicial][elemento] = wavelengths

        # Calculando estatísticas
        mean_wavelength = np.mean(wavelengths)
        median_wavelength = np.median(wavelengths)
        std_dev_wavelength = np.std(wavelengths)
        min_wavelength = np.min(wavelengths)
        max_wavelength = np.max(wavelengths)

        print(f"Arquivo: {input_file}, Atmosfera: {elemento}, Energia: {e_inicial} GeV")
        print(f"  Média: {mean_wavelength:.2e} nm")
        print(f"  Mediana: {median_wavelength:.2e} nm")
        print(f"  Desvio Padrão: {std_dev_wavelength:.2e} nm")
        print(f"  Mínimo: {min_wavelength:.2e} nm, Máximo: {max_wavelength:.2e} nm")

        # Armazenando médias para o gráfico comparativo
        mean_wavelengths[elemento].append(mean_wavelength)

# Criando os gráficos agrupados por energia
for e_inicial in energias_iniciais:
    plt.figure(figsize=(12, 8))
    
    # Verificar se há dados para esta energia
    has_data = any(len(dados_por_energia[e_inicial][elem]) > 0 for elem in elementos)
    if not has_data:
        continue
    
    # Plotar histogramas para cada elemento
    for elemento in elementos:
        wavelengths = dados_por_energia[e_inicial][elemento]
        if len(wavelengths) > 0:
            plt.hist(wavelengths, bins=50, color=cores[elemento], edgecolor='black', 
                    alpha=0.5, label=elemento, density=True)
    
    plt.xlabel("Comprimento de Onda (nm)", fontsize=12)
    plt.ylabel("Densidade Normalizada", fontsize=12)
    plt.title(f"Distribuição de Comprimentos de Onda de Raios Gama\nEnergia Inicial: {e_inicial} GeV", fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    
    # Ajustar escala se necessário (dependendo dos dados)
    if len(wavelengths) > 0:
        min_wl = min(min(wls) for wls in dados_por_energia[e_inicial].values() if len(wls) > 0)
        max_wl = max(max(wls) for wls in dados_por_energia[e_inicial].values() if len(wls) > 0)
        plt.xlim(min_wl * 0.9, max_wl * 1.1)
    
    # Salvar a figura
    output_image_file = f"histogram_combined_E{e_inicial.replace('.', '')}.png"
    plt.tight_layout()
    plt.savefig(output_image_file, dpi=300, bbox_inches='tight')
    plt.close()
    image_files.append(output_image_file)

# Criando o gráfico comparativo das médias
plt.figure(figsize=(12, 8))
for elemento, cor in cores.items():
    if len(mean_wavelengths[elemento]) == len(energias_iniciais):
        plt.plot([float(e) for e in energias_iniciais], mean_wavelengths[elemento], 
                marker='o', markersize=10, linewidth=2, label=elemento, color=cor)

plt.title("Comparação das Médias dos Comprimentos de Onda por Atmosfera", fontsize=14)
plt.xlabel("Energia Inicial (GeV)", fontsize=12)
plt.ylabel("Comprimento de Onda Médio (nm)", fontsize=12)
plt.xticks([float(e) for e in energias_iniciais], fontsize=12)
plt.yticks(fontsize=12)
plt.legend(title="Atmosfera", fontsize=12, title_fontsize=12)
plt.grid(True, linestyle='--', alpha=0.7)
plt.tight_layout()

comparative_image_file = "comparative_means.png"
plt.savefig(comparative_image_file, dpi=300, bbox_inches='tight')
plt.close()
image_files.append(comparative_image_file)

# Função para criar o PDF
def create_pdf(output_pdf, image_files):
    c = canvas.Canvas(output_pdf, pagesize=letter)
    width, height = letter

    for image_file in image_files:
        if os.path.exists(image_file):
            # Adicionar título com o nome do arquivo
            c.setFont("Helvetica-Bold", 16)
            title = os.path.basename(image_file).replace('.png', '').replace('_', ' ')
            c.drawString(100, height - 50, title)
            
            # Adicionar a imagem
            c.drawImage(image_file, 50, 100, width - 100, height - 200, preserveAspectRatio=True)
            c.showPage()

    c.save()
    print(f"PDF gerado: {output_pdf}")

output_pdf = "gamma_histograms_combined.pdf"
create_pdf(output_pdf, image_files)