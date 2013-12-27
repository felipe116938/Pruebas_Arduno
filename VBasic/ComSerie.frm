VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command2 
      Caption         =   "0"
      Height          =   375
      Left            =   3600
      TabIndex        =   4
      Top             =   1200
      Width           =   975
   End
   Begin VB.CommandButton Command1 
      Caption         =   "1"
      Height          =   375
      Left            =   2040
      TabIndex        =   3
      Top             =   1200
      Width           =   855
   End
   Begin MSCommLib.MSComm serie 
      Left            =   720
      Top             =   2160
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      DTREnable       =   -1  'True
   End
   Begin VB.Timer Timer1 
      Interval        =   20
      Left            =   960
      Top             =   1320
   End
   Begin VB.CommandButton cmdDesconectar 
      Caption         =   "DESCONECTAR"
      Height          =   495
      Left            =   2160
      TabIndex        =   1
      Top             =   240
      Width           =   1455
   End
   Begin VB.CommandButton cmdConectar 
      Caption         =   "CONECTAR"
      Height          =   495
      Left            =   360
      TabIndex        =   0
      Top             =   240
      Width           =   1455
   End
   Begin VB.Label pulsador 
      Caption         =   "Label1"
      Height          =   255
      Left            =   1800
      TabIndex        =   2
      Top             =   2160
      Width           =   2175
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
 
Dim dato As String 'Variable para guarda el dato recibido por el puerto serie
 
'Botón de conectar
Private Sub cmdConectar_Click()
    serie.PortOpen = True   'Abrimos el puerto serie
    cmdConectar.Enabled = False 'Deshabilitamos botón de conectar
    cmdDesconectar.Enabled = True 'Habilitamos botón de conectar
    Timer1.Enabled = True 'Habilitar temporizador
End Sub
 
'Botón de desconectar
Private Sub cmdDesconectar_Click()
    serie.PortOpen = False  'Cerramos el puerto serie
    cmdConectar.Enabled = True 'Habilitamos botón de conectar
    cmdDesconectar.Enabled = False 'Deshabilitamos botón de desconectar
    Timer1.Enabled = False 'Deshabilitamos temporizador
End Sub
 
'Función que se realiza automáticamente cada 20 ms
' interval = 30 en Timer
'Private Sub Timer1_Timer()
'    If serie.PortOpen Then
'        dato = serie.Input 'Cortamos el dato que nos llega por puerto serie
'        'para solamente tener un único caracter
'        pulsador.Caption = dato 'Asigamos ese dato a nuestra label para mostrar el estado
        
'    End If
'End Sub

Private Sub Timer1_Timer()
If serie.PortOpen Then
    dato = Left(serie.Input, 1) 'Cortamos el dato que nos llega por puerto serie
    'para solamente tener un único caracter
    pulsador.Caption = dato 'Asigamos ese dato a nuestra label para mostrar el estado
    'del pulsador
    serie.Output = pulsador.Caption 'Enviamos al arduino nuestro dato recibido por puerto serie
    End If
End Sub







Private Sub Form_Load()
    ' Especifica el puerto a usar
    serie.CommPort = 4
End Sub
