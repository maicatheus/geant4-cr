import os
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict


h = 4.135667696e-15  
c = 2.99792458e8     

def calculate_wavelength(energy_ev):
    """Calculate wavelength in nm from energy in eV."""
    if energy_ev <= 0:
        return None
    wavelength_m = (h * c) / energy_ev 
    return wavelength_m * 1e9  

def load_and_group_files(directory):
    """Load all .hit files and group them by energy."""
    data = defaultdict(lambda: defaultdict(list))
    
    for filename in os.listdir(directory):
        if filename.endswith('.hit'):
            try:
                
                parts = filename.split('-')
                material = parts[0]
                energy = parts[1]
                seed = parts[2].split('.')[0]
                
                filepath = os.path.join(directory, filename)
                gamma_energies = []
                
                with open(filepath, 'r') as f:
                    for line in f:
                        fields = line.strip().split()
                        if len(fields) >= 12 and fields[11] == 'gamma':
                            try:
                                e_kin_ev = float(fields[10])
                                if e_kin_ev > 0:
                                    gamma_energies.append(e_kin_ev)
                            except (ValueError, IndexError):
                                continue
                
                if gamma_energies:
                    wavelengths = [w for w in (calculate_wavelength(e) for e in gamma_energies) if w is not None]
                    if wavelengths:
                        data[energy][material].extend(wavelengths)
            
            except (IndexError, ValueError) as e:
                print(f"Error processing {filename}: {str(e)}")
                continue
    
    return data

def plot_grouped_histograms(data, output_dir="analysis_results"):
    """Generate grouped histograms by energy."""
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    image_files = []
    colors = {'Air': 'blue', 'CO2': 'green', 'CH4': 'red'}
    
    for energy, materials_data in data.items():
        plt.figure(figsize=(12, 7))
        
        
        for material, wavelengths in materials_data.items():
            plt.hist(
                wavelengths,
                bins=50,
                color=colors.get(material, 'gray'),
                alpha=0.6,
                edgecolor='black',
                label=f'{material} (N={len(wavelengths)})'
            )
        
        plt.xlabel('Wavelength (nm)')
        plt.ylabel('Count')
        plt.title(f'Gamma Wavelength Distribution at {energy} GeV')
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        
        output_file = os.path.join(output_dir, f'gamma_wavelengths_{energy}GeV.png')
        plt.savefig(output_file, bbox_inches='tight', dpi=300)
        plt.close()
        image_files.append(output_file)
    
    return image_files

def plot_statistics_comparison(data, output_dir="analysis_results"):
    """Plot comparison of mean wavelengths."""
    plt.figure(figsize=(12, 7))
    
    colors = {'Air': 'blue', 'CO2': 'green', 'CH4': 'red'}
    markers = {'Air': 'o', 'CO2': 's', 'CH4': '^'}
    
    
    energies = sorted(data.keys(), key=float)
    materials = sorted(set(mat for energy_data in data.values() for mat in energy_data.keys()))
    
    for material in materials:
        means = []
        stds = []
        
        for energy in energies:
            if material in data[energy]:
                wavelengths = data[energy][material]
                means.append(np.mean(wavelengths))
                stds.append(np.std(wavelengths))
            else:
                means.append(np.nan)
                stds.append(np.nan)
        
        plt.errorbar(
            [float(e) for e in energies],
            means,
            yerr=stds,
            marker=markers.get(material, 'o'),
            color=colors.get(material, 'gray'),
            linestyle='-',
            markersize=8,
            capsize=5,
            label=material
        )
    
    plt.title('Mean Gamma Wavelength by Material and Energy')
    plt.xlabel('Energy (GeV)')
    plt.ylabel('Mean Wavelength (nm)')
    plt.xticks([float(e) for e in energies])
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.yscale('log')
    
    output_file = os.path.join(output_dir, 'wavelength_comparison.png')
    plt.savefig(output_file, bbox_inches='tight', dpi=300)
    plt.close()
    
    return output_file

def main():
    
    sim_dirs = [d for d in os.listdir('..') if d.startswith('simulation_results_')]
    if not sim_dirs:
        print("No simulation results directory found!")
        return
    
    latest_dir = sorted(sim_dirs)[-1]
    directory = os.path.join('..', latest_dir)
    print(f"Analyzing files in: {directory}")
    
    
    grouped_data = load_and_group_files(directory)
    if not grouped_data:
        print("No valid data found!")
        return
    
    
    histograms = plot_grouped_histograms(grouped_data)
    comparison = plot_statistics_comparison(grouped_data)
    
    print("\nAnalysis complete! Generated plots:")
    print(f"- Grouped histograms by energy: {len(histograms)} files")
    print(f"- Statistical comparison: {comparison}")

if __name__ == "__main__":
    main()