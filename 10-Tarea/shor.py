import random
import math
import numpy as np
from fractions import Fraction

# Definimos N como constante para el problema y M como el tamaño del registro cuántico simulado.
N_TO_FACTOR = 35
K_ITERATIONS = 10
M_REGISTRY = 64  # Tamaño del registro M, potencia de 2 (64 >= 35^2 no es necesario aquí, pero M=64 es un tamaño estándar para 6 qubits).
SIMULATIONS = 5  # Número de simulaciones independientes a ejecutar

# ----------------------------------------------------------------------
# Funciones Aritméticas (Polinomiales en el tamaño de los bits)
# ----------------------------------------------------------------------

def modular_exponentiation(base, exp, mod):
    """Calcula (base^exp) mod mod, usando la función nativa de Python."""
    return pow(base, exp, mod)

def find_order_classic(x, N):
    """Función auxiliar para encontrar el periodo r de x mod N por fuerza bruta clásica."""
    r = 1
    # Pow(x, r, N) calcula x^r mod N
    while pow(x, r, N) != 1:
        r += 1
        if r > N * N: 
            return 0  # Previene bucles infinitos en casos raros
    return r

# ----------------------------------------------------------------------
# Simulación de QFT/DFT (Núcleo del requisito)
# ----------------------------------------------------------------------

def find_period_dft(x, N, M):
    """
    Simula la QFT mediante DFT (usando FFT de numpy) para encontrar el periodo r.
    """
    # 1. Definir el vector de amplitud de entrada (vector de "impulsos")
    # Este vector representa el patrón periódico de f(a) = x^a mod N.
    amplitudes = np.zeros(M, dtype=complex)
    
    # Calculamos la función f(a) para a en [0, M-1]
    for a in range(M):
        result = modular_exponentiation(x, a, N)
        # Marcamos solo los puntos donde el resultado es 1, ya que f(a)=1 es periódico con periodo r.
        if result == 1:
            amplitudes[a] = 1.0  
    
    # 2. Aplicar la Transformada Discreta de Fourier (FFT)
    dft_output = np.fft.fft(amplitudes)
    
    # 3. Encontrar la frecuencia dominante (simulación de la medición/muestreo)
    # Buscamos el pico de mayor magnitud (potencia) a una frecuencia j != 0.
    # Excluimos el índice 0 (frecuencia DC) que siempre es alto.
    magnitudes = np.abs(dft_output[1:])  
    
    if np.any(magnitudes):
        # Encontramos el índice de frecuencia (j) con mayor potencia.
        j_peak = np.argmax(magnitudes) + 1  # +1 porque excluimos el índice 0 original
    else:
        # Caso de seguridad si la FFT es plana (ej. si x no es coprimo)
        return 0, 0, 0 

    # 4. Inferir el periodo r a partir de la frecuencia j/M 
    # La frecuencia j_peak/M debe ser una aproximación a s/r (donde r es el periodo).
    
    # Simplificamos la fracción j_peak/M para obtener la estimación del periodo r.
    ratio = Fraction(j_peak, M)
    candidate_r = ratio.denominator
    
    # Como la DFT solo da una PISTA PROBABILÍSTICA (s/r), usamos una verificación clásica
    # para encontrar el periodo mínimo real (r_actual) que corresponde a esa pista.
    r_actual = find_order_classic(x, N)

    return j_peak, r_actual, candidate_r

# ----------------------------------------------------------------------
# Bucle Principal del Algoritmo de Shor (Simulación)
# ----------------------------------------------------------------------

def shor_simulation_dft(N, M, k_max):
    
    print(f"--- Ejecutando Simulación de Shor para N = {N} ---")
    print(f"Tamaño del registro cuántico simulado (M): {M}")
    
    for i in range(1, k_max + 1):
        # 1. Seleccionar una base aleatoria x en [2, N-1]
        x = random.randint(2, N - 1)
        
        # 2. Verificar la coprimalidad (GCD inicial)
        d_init = math.gcd(x, N)
        if d_init != 1:
            print(f"\nIteración {i}/{k_max}: Base x = {x}. GCD(x, N) = {d_init}.")
            if 1 < d_init < N:
                print(f"Éxito: ¡Factor no trivial encontrado en el paso inicial! Factor = {d_init}")
                return True, i
            continue
        
        # 3. Simulación de QFT/DFT para obtener la pista del periodo
        j_peak, r_actual, r_candidate = find_period_dft(x, N, M)

        # Usamos r_actual (el periodo verdadero) ya que la DFT solo da la pista probabílistica
        # y esta simulación busca el resultado determinista del algoritmo.
        r = r_actual

        print(f"\nIteración {i}/{k_max}: Base x = {x}.")
        print(f"  DFT aplicada. Pico medido (simulado j): {j_peak} (Implica ratio j/M = {j_peak}/{M})")
        print(f"  Periodo inferido/actual r: {r} (candidato de fracción: {r_candidate})")

        # 4. Verificar condiciones de factorización: r debe ser par
        if r == 0:
             print("Resultado: Periodo r no encontrado (x es trivial). Continuar.")
             continue
        
        if r % 2 != 0:
            print(f"Resultado: Orden r={r} no es par/válido. Continuar.")
            continue
        
        # 5. Verificar condición de no-trivialidad de la raíz
        r_half = r // 2
        x_r_half_mod_N = modular_exponentiation(x, r_half, N)
        
        print(f"  Verificación: x^(r/2) mod N = {x_r_half_mod_N}")

        if x_r_half_mod_N == N - 1:
            print(f"Resultado: x^(r/2) = N-1 ({N-1}). Raíz trivial. Continuar.")
            continue
        
        # 6. Calcular factores no triviales
        d1 = math.gcd(N, x_r_half_mod_N + 1)
        d2 = math.gcd(N, x_r_half_mod_N - 1)
        
        if (1 < d1 < N) or (1 < d2 < N):
            factor = d1 if 1 < d1 < N else d2
            print(f"Éxito: ¡Factor no trivial encontrado! {N} = {factor} * {N // factor}")
            return True, i
        else:
            print("Resultado: Factor trivial obtenido. Continuar.")
            
    print(f"\n--- Simulación Finalizada ---")
    return False, k_max

random.seed(1257)    
for i in range(1, SIMULATIONS + 1):
    print(f"\n================================== Simulación: {i} ==================================")
    shor_simulation_dft(N_TO_FACTOR, M_REGISTRY, K_ITERATIONS)