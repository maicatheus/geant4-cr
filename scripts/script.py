import os
import numpy as np
import matplotlib.pyplot as plt
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import letter

h = 4.135667696e-15  #(eV*s)
c = 3e8  # (m/s)

GeV_to_eV = 1e9  #ara eV

def calculate_wavelength(energy_gev):
    if energy_gev <= 0:  #válida
        return None
    energy_ev = energy_gev# * GeV_to_eV  #Para eV
    wavelength_m = (h * c) / energy_ev 
    wavelength_nm = wavelength_m * 1e9  #metros
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
for elemento in elementos:
    for e_inicial in energias_iniciais:
        e = e_inicial.replace(".", "")
        output_file_name = f"{elemento}_E0_{e}_n_100"

        input_file = output_file_name + ".hit"

        if not os.path.exists(input_file):
            print(f"Arquivo {input_file} não encontrado. Pulando...")
            continue

        gamma_energies = []

        #s gama
        with open(input_file, "r") as file:
            for line in file:
                data = line.strip().split()
                particle = data[11]  
                e_kin_gev = float(data[10])  # (eV)
                if particle == "gamma" and e_kin_gev > 0:  
                    gamma_energies.append(e_kin_gev)


        wavelengths = [calculate_wavelength(e) for e in gamma_energies if calculate_wavelength(e) is not None]

        if not wavelengths: 
            print(f"Nenhum dado relevante encontrado no arquivo {input_file}. Pulando...")
            continue


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

    
        plt.figure(figsize=(10, 6))
        plt.hist(wavelengths, bins=50, color=cores.get(elemento), edgecolor='black', alpha=0.7)
        plt.axvline(mean_wavelength, color='orange', linestyle='dashed', linewidth=2, label=f'Média: {mean_wavelength:.2e} nm')
        plt.axvline(median_wavelength, color='purple', linestyle='dashed', linewidth=2, label=f'Mediana: {median_wavelength:.2e} nm')
        plt.xlabel("Comprimento de Onda (nm)")
        plt.ylabel("Contagem")
        plt.title(f"Segmento de gamma em {elemento} - Energia: {e_inicial} GeV")
        plt.legend()
        plt.grid(True)


        output_image_file = output_file_name + ".png"
        plt.savefig(output_image_file)
        plt.close()
        image_files.append(output_image_file)


plt.figure(figsize=(10, 6))
for elemento, cor in cores.items():
    plt.plot([float(e) for e in energias_iniciais], mean_wavelengths[elemento], marker='o', label=elemento, color=cor)

plt.title("Comparação das Médias dos Comprimentos de Onda por Atmosfera")
plt.xlabel("Energia Inicial (GeV)")
plt.ylabel("Comprimento de Onda Médio (nm)")
plt.xticks([float(e) for e in energias_iniciais])
plt.legend(title="Atmosfera")
plt.grid(True)
comparative_image_file = "comparative_means.png"
plt.savefig(comparative_image_file)
plt.close()
image_files.append(comparative_image_file)

# o PDF

def create_pdf(output_pdf, image_files):
    c = canvas.Canvas(output_pdf, pagesize=letter)
    width, height = letter

    for image_file in image_files:
        if os.path.exists(image_file):
            c.drawImage(image_file, 0, 0, width, height, preserveAspectRatio=True)
            c.showPage()

    c.save()
    print(f"PDF gerado: {output_pdf}")

output_pdf = "gamma_histograms.pdf"
create_pdf(output_pdf, image_files)
