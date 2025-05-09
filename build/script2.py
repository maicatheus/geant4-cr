import os
import numpy as np
import matplotlib.pyplot as plt
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import letter

# Constantes físicas
h = 4.135667696e-15  # (eV*s)
c = 3e8  # (m/s)
GeV_to_eV = 1e9  # Conversão GeV para eV
MeV_to_GeV = 1e-3  # Conversão MeV para GeV
MeV_to_eV = 1e6  # Conversão MeV para eV

def calculate_wavelength(energy_ev):
    if energy_ev <= 0:
        return None
    energy_ev = energy_ev  # * GeV_to_eV
    wavelength_m = (h * c) / energy_ev 
    wavelength_nm = wavelength_m  # * 1e9  # metros para nanômetros
    return wavelength_nm

# Configurações
energias_iniciais = ["0.5", "1", "5", "10"]  # Energias iniciais em GeV
elementos = ["AIR", "CO2", "CH4"]
cores = {
    "AIR": "blue",
    "CO2": "green",
    "CH4": "red"
}

# Processamento dos dados
image_files = []
mean_wavelengths = {elemento: [] for elemento in elementos}
all_wavelengths = {e: {elem: [] for elem in elementos} for e in energias_iniciais}

for elemento in elementos:
    for e_inicial in energias_iniciais:
        e = e_inicial.replace(".", "")
        output_file_name = f"{elemento}_E0_{e}_n_100"
        input_file = output_file_name + ".hit"

        if not os.path.exists(input_file):
            print(f"Arquivo {input_file} não encontrado. Pulando...")
            continue

        gamma_energies = []

        # Leitura dos fótons gama
        with open(input_file, "r") as file:
            for line in file:
                data = line.strip().split()
                if len(data) < 12:
                    continue
                particle = data[11]
                try:
                    e_kin_mev = float(data[10])  # Energia em MeV
                    e_kin_gev = e_kin_mev * MeV_to_eV  # Convertendo para GeV
                except (ValueError, IndexError):
                    continue
                
                if particle == "gamma" and e_kin_gev > 0:
                    gamma_energies.append(e_kin_gev)

        wavelengths = [calculate_wavelength(e) for e in gamma_energies if calculate_wavelength(e) is not None]
        
        if not wavelengths:
            print(f"Nenhum dado relevante encontrado no arquivo {input_file}. Pulando...")
            continue

        # Armazena os comprimentos de onda para o histograma combinado
        all_wavelengths[e_inicial][elemento] = wavelengths

        # Cálculos estatísticos
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

        mean_wavelengths[elemento].append(mean_wavelength)

# Gráficos combinados para cada energia
for e_inicial in energias_iniciais:
    if not any(all_wavelengths[e_inicial][elem] for elem in elementos):
        continue
    
    # Gráfico combinado padrão
    plt.figure(figsize=(12, 7))
    for elemento in elementos:
        if all_wavelengths[e_inicial][elemento]:
            plt.hist(all_wavelengths[e_inicial][elemento], bins=50, 
                    color=cores.get(elemento), edgecolor='black', alpha=0.5,
                    label=elemento)
    
    plt.xlabel("Comprimento de Onda (nm)")
    plt.ylabel("Contagem")
    plt.title(f"Distribuição Combinada - Energia: {e_inicial} GeV")
    plt.legend()
    plt.grid(True)
    
    combined_file = f"combined_{e_inicial.replace('.', '')}.png"
    plt.savefig(combined_file)
    plt.show()  # Mostrar o gráfico na tela
    plt.close()
    image_files.append(combined_file)
    
    # Gráfico combinado com xlim personalizado e ylim ajustado
    plt.figure(figsize=(12, 7))
    
    # Definir os limites de x para cada energia
    xlim_rules = {
        "0.5": (3.0e-11, 7.0e-11),
        "1": (4.5e-11, 8.e-11),
        "5": (0.3e-10, 1.75e-10),
        "10": (0.6e-10, 4e-10)
    }
    
    xmin, xmax = xlim_rules.get(e_inicial)
    
    max_count = 0
    bin_edges = None
    
    # Primeiro passada para calcular o ylim adequado
    for elemento in elementos:
        if all_wavelengths[e_inicial][elemento]:
            counts, bins = np.histogram(all_wavelengths[e_inicial][elemento], bins=50)
            bin_centers = (bins[:-1] + bins[1:]) / 2
            mask = (bin_centers >= xmin) & (bin_centers <= xmax)
            if any(mask):
                current_max = np.max(counts[mask])
                if current_max > max_count:
                    max_count = current_max
            if bin_edges is None:
                bin_edges = bins
    
    # Segunda passada para plotar os histogramas
    for elemento in elementos:
        if all_wavelengths[e_inicial][elemento]:
            plt.hist(all_wavelengths[e_inicial][elemento], bins=bin_edges,
                    color=cores.get(elemento), edgecolor='black', alpha=0.5,
                    label=elemento)
    
    plt.xlabel("Comprimento de Onda (nm)")
    plt.ylabel("Contagem")
    plt.title(f"Distribuição Combinada - {e_inicial} GeV\nxlim: [{xmin:.1e}, {xmax:.1e}] nm")
    # Aplicar os limites
    plt.xlim(left=xmin, right=xmax)
    if max_count > 0:
        plt.ylim(top=max_count * 1.1)  # 10% de margem acima do valor máximo
    
    plt.legend()
    plt.grid(True)
    
    combined_custom_file = f"combined_{e_inicial.replace('.', '')}_custom.png"
    plt.savefig(combined_custom_file)
    plt.show()  # Mostrar o gráfico na tela
    plt.close()
    image_files.append(combined_custom_file)

# Gráfico comparativo
valid_elements = [elem for elem in elementos if len(mean_wavelengths[elem]) == len(energias_iniciais)]
if valid_elements:
    plt.figure(figsize=(12, 7))
    for elemento in valid_elements:
        plt.plot([float(e) for e in energias_iniciais], mean_wavelengths[elemento], 
                marker='o', markersize=8, linestyle='-', linewidth=2,
                label=elemento, color=cores.get(elemento))
    
    plt.title("Comparação dos Comprimentos de Onda Médios")
    plt.xlabel("Energia Inicial (GeV)")
    plt.ylabel("Comprimento de Onda Médio (nm)")
    plt.xticks([float(e) for e in energias_iniciais])
    plt.legend(title="Atmosfera")
    plt.grid(True)
    
    comparative_file = "comparative_means.png"
    plt.savefig(comparative_file)
    plt.show()  # Mostrar o gráfico na tela
    plt.close()
    image_files.append(comparative_file)

# Gerar PDF
def create_pdf(output_pdf, image_files):
    c = canvas.Canvas(output_pdf, pagesize=letter)
    width, height = letter

    for image_file in image_files:
        if os.path.exists(image_file):
            img_width = width - 100
            img_height = height - 100
            c.drawImage(image_file, 50, 50, img_width, img_height, preserveAspectRatio=True)
            c.showPage()

    c.save()
    print(f"PDF gerado: {output_pdf}")

output_pdf = "gamma_analysis_report.pdf"
create_pdf(output_pdf, image_files)

print("Análise concluída com sucesso!")