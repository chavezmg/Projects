import tkinter as tk
from PIL import ImageTk, Image
import random

root = tk.Tk()
root.title("Piedra Papel Tijera")
root.resizable(False, False)

eleccionP1 = ""
score = {"Victorias": 0, 
         "Derrotas": 0, 
         "Empates": 0}
botones = ["Piedra", "Papel", "Tijera"]
   
def reset():
    global score
    global marcador  
    score = {"Victorias": 0, 
         "Derrotas": 0, 
         "Empates": 0}
    marcador = tk.Label(frameMarcador, text=f"{score}").grid(row=6, column=0)

def button(root, text, fila, columna, command, width=10, height=3):
    but = tk.Radiobutton(root, text=text, variable = eleccionP1, command = command, width=width, height=height, value=text)
    but.grid(row = fila, column = columna, padx=10, pady=5)
def jugar():
    global score
    global eleccionP1
    global resultado
    global marcador
    eleccionPC = random.choice(botones)
    if eleccionPC == "Piedra":
        player2 = tk.Label(frameVersus, image = imgPiedra2).grid(row=1, column=5)

        if (eleccionP1 == "Piedra"):
            score["Empates"]+=1
            resultado = tk.Label(frameVersus, text=" Empate! ", fg="gold",font=("Arial", 15)).grid(row=0, column=3)
        elif (eleccionP1 == "Papel"):
            score["Victorias"]+=1
            resultado = tk.Label(frameVersus, text="Victoria!", fg="gold",font=("Arial", 15)).grid(row=0, column=3)
        elif (eleccionP1 == "Tijera"):
            score["Derrotas"]+=1
            resultado = tk.Label(frameVersus, text="Derrota!", fg="gold",font=("Arial", 15)).grid(row=0, column=3)

    elif eleccionPC == "Papel":
        player2 = tk.Label(frameVersus, image = imgPapel2).grid(row=1, column=5)
        if (eleccionP1 == "Piedra"):
            score["Derrotas"]+=1
            resultado = tk.Label(frameVersus, text="Derrota!", fg="gold",font=("Arial", 15)).grid(row=0, column=3)
        elif (eleccionP1 == "Papel"):
            score["Empates"]+=1
            resultado = tk.Label(frameVersus, text=" Empate! ", fg="gold",font=("Arial", 15)).grid(row=0, column=3)
        elif (eleccionP1 == "Tijera"):
            score["Victorias"]+=1
            resultado = tk.Label(frameVersus, text="Victoria!", fg="gold",font=("Arial", 15)).grid(row=0, column=3)

    elif eleccionPC == "Tijera": 
        player2 = tk.Label(frameVersus, image = imgTijera2).grid(row=1, column=5)
        if (eleccionP1 == "Piedra"):
            score["Victorias"]+=1
            resultado = tk.Label(frameVersus, text="Victoria!", fg="gold",font=("Arial", 15)).grid(row=0, column=3)
        elif (eleccionP1 == "Papel"):
            score["Derrotas"]+=1
            resultado = tk.Label(frameVersus, text="Derrota!", fg="gold",font=("Arial", 15)).grid(row=0, column=3)
        elif (eleccionP1 == "Tijera"):
            score["Empates"]+=1    
            resultado = tk.Label(frameVersus, text=" Empate! ", fg="gold",font=("Arial", 15)).grid(row=0, column=3)
    print(f"jugador:{eleccionP1}, PC:{eleccionPC}")
    marcador = tk.Label(frameMarcador, text=f"{score}").grid(row=6, column=0)
    print(f"jugador:{eleccionP1}, PC:{eleccionPC}")

    
def Piedra():
    global eleccionP1
    player1 = tk.Label(frameVersus, image = imgPiedra).grid(row=1, column=0)
    player2 = tk.Label(frameVersus, image = imgInterr).grid(row=1, column=5)
    eleccionP1 = "Piedra"

def Papel():
    global eleccionP1
    player1 = tk.Label(frameVersus, image = imgPapel).grid(row=1, column=0)
    player2 = tk.Label(frameVersus, image = imgInterr).grid(row=1, column=5)
    eleccionP1 = "Papel"

def Tijera():
    global eleccionP1
    player1 = tk.Label(frameVersus, image = imgTijera).grid(row=1, column=0)
    player2 = tk.Label(frameVersus, image = imgInterr).grid(row=1, column=5)
    eleccionP1 = "Tijera"

# imagenes
imgPiedra = ImageTk.PhotoImage(Image.open(".\\imagenes\\piedra.png"))
imgPapel = ImageTk.PhotoImage(Image.open(".\\imagenes\\papel.png"))
imgTijera = ImageTk.PhotoImage(Image.open(".\\imagenes\\tijera.png"))
imgPiedra2 = ImageTk.PhotoImage(Image.open(".\\imagenes\\piedra2.png"))
imgPapel2 = ImageTk.PhotoImage(Image.open(".\\imagenes\\papel2.png"))
imgTijera2 = ImageTk.PhotoImage(Image.open(".\\imagenes\\tijera2.png"))
imgInterr = ImageTk.PhotoImage(Image.open(".\\imagenes\\interrogacion.png"))
#

#creamos el gui versus
frameVersus = tk.LabelFrame(root, text="Piedra Papel Tijera", bd=3)
frameVersus.grid(row=0, column=0, columnspan=5, padx=5,pady=5)
p1 = tk.Label(frameVersus, text="Player 1", fg="red",font=("Arial", 15)).grid(row=0, column=0)
resultado = tk.Label(frameVersus, text="Resultado", fg="gold",font=("Arial", 15)).grid(row=0, column=3)
p2 = tk.Label(frameVersus, text="PC", fg="blue",font=("Arial", 15)).grid(row=0, column=5)
player1 = tk.Label(frameVersus, image = imgInterr).grid(row=1, column=0)
versus = tk.Label(frameVersus, text = "Versus",font=("Arial", 25)).grid(row=1, column=3, padx=20)
player2 = tk.Label(frameVersus, image = imgInterr).grid(row=1, column=5)

#creamos botones piedra papel tijera, listo y reset
frameBotones = tk.LabelFrame(root, text = "Opciones",bd=5, bg="lightgrey")
frameBotones.grid(row=2,column=0, rowspan=2, columnspan=3, padx=5,pady=5)
i,j = 2,0
for boton in botones:
    button(frameBotones, boton, fila = i, columna = j, command=eval(boton))
    j+=1
listo = tk.Button(frameBotones, text="Listo", width=10, height=2, fg="green", command=jugar).grid(row=3, column=1, pady=15)
reset = tk.Button(frameBotones, text="Reset", width=10, height=2, fg="red", command=reset).grid(row=3, column=2, pady=15)
#

#creamos el marcador
frameMarcador = tk.LabelFrame(root, text="Marcador", bd=3, fg="red")
frameMarcador.grid(row=6,column=0, columnspan=5)
#marcador = tk.Label(frameMarcador, text=f"{score}").grid(row=6, column=0)
root.mainloop()

