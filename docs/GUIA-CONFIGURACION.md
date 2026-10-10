# Dilemma Max V3: guía de configuración

Esta guía describe todos los cambios que se hicieron al firmware del teclado, cómo probarlos y cómo modificarlos.

> **Estado:** todo compila sin errores y la lógica de los gestos se probó en la computadora. **Nada se ha probado todavía en el teclado**, así que es normal que haya que ajustar algunos valores.

---

## 1. Resumen de lo que se hizo

| # | Cambio | Para qué sirve |
|---|---|---|
| 1 | Pin serial movido de `GP1` a `GP16` | Repara la comunicación entre mitades. Se hizo con un cable puente porque `GP1` se dañó. |
| 2 | VIA activado | Permite reasignar teclas desde [usevia.app](https://usevia.app) |
| 3 | Swipes de 3 dedos con atajos de Mac | Cambiar de escritorio, Mission Control y App Exposé |
| 4 | Velocidad del cursor ajustable (25% por defecto) | Cursor más preciso, ajustable desde el teclado |
| 5 | Zoom con dos dedos (pellizcar o separar) | Zoom en Figma, AutoCAD y otras apps |
| 6 | Diferenciar zoom de scroll por dirección | Evita que el scroll active el zoom |
| 7 | Gesto desde el borde izquierdo con dos dedos | Atrás en el navegador |
| 8 | Movimiento rápido con dos dedos | Atrás y adelante en el navegador desde cualquier parte |
| 9 | Tema de luces Catppuccin Mocha | Iluminación con la paleta oficial de Catppuccin |

---

## 2. Archivos importantes

| Qué | Dónde |
|---|---|
| Firmware listo para flashear | `~/Downloads/dilemma/dilemma_max_mac_catppuccin.uf2` |
| Código fuente del firmware | `~/bastardkb-qmk` (rama `feat/mac-trackpad-gestures`) |
| **Configuración de gestos (el archivo que vas a editar)** | `~/bastardkb-qmk/keyboards/bastardkb/dilemma/4x6_4_procyon/keymaps/mac/config.h` |
| Teclas personalizadas y capas | `.../keymaps/mac/keymap.c` |
| Efecto de luces Catppuccin | `.../keymaps/mac/rgb_matrix_user.inc` |
| Documentación técnica (inglés) | `~/Downloads/dilemma/trackpad-mac-gestures-readme.md` |
| Registro de tareas | `~/Downloads/dilemma/odd/tasks/mac-trackpad-gestures.md` |

---

## 3. Cómo flashear el firmware

Cada mitad tiene su propio chip, así que **siempre se flashean por separado**, una a la vez y con el mismo archivo `.uf2`:

1. **Desconectá el cable de audio (TRRS)** entre las mitades.
2. Conectá el USB a una mitad.
3. Entrá al modo de carga con alguna de estas dos opciones:
   - **Con el teclado (`QK_BOOT`)**. Es lo más cómodo:

     | Mitad con el USB | Mantené | Y apretá |
     |---|---|---|
     | **Izquierda** | **MO(1)** (LOWER: el pulgar izquierdo más cercano al centro) | **B** |
     | **Derecha** | **MO(2)** (RAISE: el pulgar derecho más cercano al centro) | **N** |

   - **Doble toque rápido en el botón de reset.** Usá esta opción si la mitad no responde porque tiene un firmware roto: en ese caso `QK_BOOT` no funciona.
4. Aparece una unidad llamada `RPI-RP2`. Arrastrá el archivo `.uf2` a esa unidad. El teclado se reinicia solo.
5. Repetí con la otra mitad.

> `QK_BOOT` **no flashea las dos mitades a la vez**. Solo reinicia en modo de carga la mitad que tiene el USB: reemplaza al doble toque del botón de reset.
>
> **Cuidado con la tecla vecina:** LOWER + **V** y RAISE + **M** son `EE_CLR`, que borra los ajustes guardados (layout de VIA, velocidad del cursor, modo de zoom y luces).

**Al terminar, conectá el TRRS primero y el USB después.** Nunca conectes ni desconectes el TRRS con el USB enchufado: eso fue lo que dañó el pin `GP1`.

> **Importante:** conectá el USB a la **mitad del trackpad** (la derecha). El cursor y el scroll funcionan desde cualquier lado. Los swipes, el zoom y los atajos solo llegan al Mac desde la mitad que tiene el USB.

### Primer arranque

La primera vez que arranques este firmware se **resetea una sola vez**:
- el layout de VIA,
- la velocidad del cursor,
- el modo de zoom,
- las luces.

Es intencional: VIA solo resetea el layout cuando cambia la *fecha* de compilación (`quantum/via.c:71-74`). Por eso, un firmware anterior compilado el mismo día dejó las teclas nuevas sin asignar.

---

## 4. Atajos de macOS que usa el firmware

No hace falta configurar nada. Estos atajos ya vienen activados por defecto en macOS:
- `Ctrl+flechas` para escritorios, Mission Control y App Exposé.
- Atrás y adelante se envían como **botones 4 y 5 del mouse** (no como `Cmd+[` / `Cmd+]`, que con la distribución de teclado en español se convierten en tilde y zoom). Funciona en Brave, Chrome, Arc, Edge y Firefox; no en Safari.

Si alguno no responde, revisá que esté activado en esa misma pantalla.

---

## 5. Gestos del trackpad y cómo probarlos

### 5.1 Un dedo

| Gesto | Acción | Cómo probarlo |
|---|---|---|
| Mover | Mueve el cursor | Debería arrancar **lento** (25%) |
| Tocar | Click | Toque corto, sin mover el dedo |
| Doble toque y arrastrar | Arrastrar | Tocá, volvé a apoyar y mové |

### 5.2 Dos dedos

| Gesto | Acción | Cómo probarlo |
|---|---|---|
| Tocar con 2 dedos | Click derecho | Toque corto con dos dedos |
| Mover los 2 dedos **hacia el mismo lado** | Scroll | En cualquier página web |
| **Separar o juntar** los dedos | Zoom | En Figma. Mové **los dos** dedos, no uno solo. |
| **Movimiento rápido →** | Atrás (botón 4 del mouse) | En el navegador, en cualquier parte del trackpad |
| **Movimiento rápido ←** | Adelante (botón 5 del mouse) | En el navegador |
| Movimiento lento horizontal | Scroll horizontal | En Figma o en una planilla ancha |
| Desde el **borde izquierdo** hacia la derecha | Atrás (botón 4 del mouse) | Dedos pegados al borde, deslizá ~9 mm |

Cómo decide el firmware qué gesto hiciste con dos dedos:

1. **Primero mira si arrancaste en un borde.** Si el punto medio entre tus dedos arranca a menos de ~10 mm del borde (el 25% del ancho), tiene prioridad el gesto de borde.
2. **Después espera a que muevas los dedos ~1.2 mm** y decide:
   - Si los dedos van en **direcciones opuestas**, es **zoom**.
   - Si van **hacia el mismo lado**, es **scroll**.
   - Si **un dedo queda quieto**, cuenta como **scroll**.
3. **Si el movimiento es horizontal**, espera hasta 100 ms:
   - Si recorrés 6 mm en ese tiempo, es **atrás/adelante**.
   - Si no, es **scroll horizontal**.
4. **Una vez decidido, el gesto queda bloqueado** hasta que levantes los dedos.

### 5.3 Tres dedos

| Gesto | Acción |
|---|---|
| Swipe ← | Escritorio siguiente (`Ctrl+→`) |
| Swipe → | Escritorio anterior (`Ctrl+←`) |
| Swipe ↑ | Mission Control (`Ctrl+↑`) |
| Swipe ↓ | App Exposé (`Ctrl+↓`) |

---

## 6. Teclas especiales (capa RAISE)

Mantené **RAISE** (`MO(2)` en VIA: el pulgar derecho más cercano al centro, al lado de `Enter`) y presioná:

| Tecla | Función | Acción |
|---|---|---|
| **Y** | `CURSOR_SPEED_DOWN` | Cursor más lento |
| **U** | `CURSOR_SPEED_UP` | Cursor más rápido |
| **I** | `PINCH_MOD_CYCLE` | Cambia el modo de zoom |
| **N** | `QK_BOOT` | Entra en modo de carga de firmware (mitad derecha) |
| **M** | `EE_CLR` | Borra la memoria y vuelve a los valores por defecto |

Desde la capa **LOWER** (`MO(1)`: el pulgar izquierdo más cercano al centro):

| Tecla | Función | Acción |
|---|---|---|
| **T** | `RGB_THEME_RESTORE` | **Vuelve al tema Catppuccin** con el brillo por defecto, sin tocar tu layout ni la velocidad |
| **B** | `QK_BOOT` | Entra en modo de carga de firmware (mitad izquierda) |
| **V** | `EE_CLR` | Borra la memoria y vuelve a los valores por defecto |
| **Shift izquierdo** | `RM_TOGG` | Prende o apaga las luces |
| **Tab** | `RM_NEXT` | Siguiente efecto de luz |
| **Ctrl izquierdo** | `RM_PREV` | Efecto de luz anterior |

### Capa 8: temas y luces (LOWER + RAISE)

Mantené **los dos pulgares del centro a la vez** (LOWER y RAISE) y usá la mano derecha:

| Tecla | Función | Acción |
|---|---|---|
| **Y** | `RGB_THEME_RESTORE` | Tema **Catppuccin** con el brillo por defecto |
| **U** | `RGB_THEME_GRUVBOX` | Tema **Gruvbox Dark** (mantiene el brillo) |
| **I** | `RGB_THEME_ALEGRA` | Tema **Alegra** (mantiene el brillo) |
| **K** | `RM_TOGG` | Prende o apaga las luces |
| **H** / **J** | `RM_VALD` / `RM_VALU` | Baja / sube el brillo (tope 128) |
| **N** / **M** | `RM_PREV` / `RM_NEXT` | Efecto anterior / siguiente |
| **O** | `RGB_THEME_DESERT` | Tema **Desierto** (mantiene el brillo) |
| **P** | `RGB_THEME_HORIZON` | Tema **Horizon** (mantiene el brillo) |
| **9** / **0** | `RM_HUED` / `RM_HUEU` | Tono (solo efectos de VIA, no los temas) |
| **L** / **;** | `RM_SATD` / `RM_SATU` | Saturación (solo efectos de VIA) |
| **,** / **.** | `RM_SPDD` / `RM_SPDU` | Velocidad del efecto (solo efectos de VIA) |

El brillo máximo quedó limitado a **128** (el teclado permitía 176), así ni las teclas ni VIA pueden volver a provocar la caída de voltaje.

> Estas posiciones corresponden al layout por defecto. Si las movés en VIA, valen las que hayas asignado vos.

**Velocidades del cursor** (de más lenta a más rápida):
12.5% → 19% → **25% (por defecto)** → 37% → 50% → 75% → 100% → 150% → 200%

**Modos de zoom** (cada toque de `PINCH_MOD_CYCLE` pasa al siguiente):
1. `Cmd` + scroll: **por defecto**, para Figma.
2. `Ctrl` + scroll.
3. Scroll solo, para AutoCAD.

La velocidad y el modo de zoom **se guardan** aunque desconectes el teclado.

**Si usás VIA** y querés mover estas teclas a otra posición, usá la tecla **"Any"** con estos códigos:

| Tecla | Código |
|---|---|
| `CURSOR_SPEED_DOWN` | `0x7E40` |
| `CURSOR_SPEED_UP` | `0x7E41` |
| `PINCH_MOD_CYCLE` | `0x7E42` |
| `RGB_THEME_RESTORE` | `0x7E43` |
| `RGB_THEME_GRUVBOX` | `0x7E44` |
| `RGB_THEME_ALEGRA` | `0x7E45` |
| `RGB_THEME_DESERT` | `0x7E46` |
| `RGB_THEME_HORIZON` | `0x7E47` |

---

## 7. Luces Catppuccin Mocha

- **Colores:** los 14 acentos oficiales de Catppuccin Mocha con sus valores hex exactos. Forman un degradé simétrico entre las dos mitades: Rosewater en los bordes exteriores y Lavender en el centro.
- **Onda al presionar una tecla:** un anillo que **oscurece** las teclas por las que pasa (hasta ~82% en el punto más fuerte). Sale de la tecla apretada, cruza las dos mitades en ~1.3 s y se va desvaneciendo mientras avanza.
- **Brillo:** se controla desde VIA (pestaña *Lighting*). El valor por defecto es 64 de 255. El tema Catppuccin, además, se atenúa al ~63% para consumir lo mismo que un color saturado de VIA (ver sección 10).

**Cómo probarlo:** con las dos mitades conectadas, las luces deben verse con la paleta (no rojas) y el trackpad debe funcionar.

### Tema alternativo: Gruvbox Dark

- **Colores:** los acentos brillantes de Gruvbox Dark (Red, Orange, Yellow, Green, Aqua, Blue y Purple). Van en degradé desde los bordes hacia el centro, con la misma onda que Catppuccin.
- **Cómo activarlo:** desde Catppuccin, **LOWER + Tab** (siguiente efecto) pasa a Gruvbox.
- **Cómo volver a Catppuccin:** **LOWER + T**.
- **Consumo:** Gruvbox es más saturado que Catppuccin (58% de carga contra 79%), así que solo se atenúa al ~87% (`GRUVBOX_POWER_SCALE` `221`).

### Tema alternativo: Alegra

- **Colores:** el **Índigo** secundario `#514EA6` solo en los bordes exteriores, y el **Teal** primario `#30ABA9` en todo el resto de cada mitad, para que predomine. En los LEDs el índigo se ve como un azul fuerte. Tiene la misma onda que los demás temas.
- **Cómo activarlo:** LOWER + RAISE + **I**.
- **Consumo:** 49% de carga, ya igual que un color saturado, así que no se atenúa (`ALEGRA_POWER_SCALE` `255`).
- **Para ajustar la proporción:** en `rgb_matrix_user.inc`, la lista `alegra_palette` tiene 1 Índigo y 4 Teal. Agregar más Teal achica todavía más la franja índigo.

### Tema alternativo: Desierto

- **Origen:** una paleta propia con la misma forma que Horizon: un cañón de roca roja al atardecer. Reemplazó a Kanagawa Dragon.
- **Degradé (desde los bordes exteriores hacia el centro):**

  | Color | Hex |
  |---|---|
  | Arena clara | `#F6D69A` |
  | Arena dorada | `#F2C14E` |
  | Duna naranja | `#F4A259` |
  | Ámbar | `#E07A3F` |
  | Terracota | `#C8553D` |
  | Rojo cañón | `#A33A2A` |

- **Cómo activarlo:** LOWER + RAISE + **O**.
- **Consumo:** 58% de carga, atenuado al ~87% (`DESERT_POWER_SCALE` `222`).
- **Para cambiar colores:** editá la lista `desert_palette` en `rgb_matrix_user.inc`. Va del borde exterior al centro.

### Tema alternativo: Horizon

- **Origen:** la paleta oficial de [jolaleye/horizon-theme-vscode](https://github.com/jolaleye/horizon-theme-vscode), archivo `src/dark/globals.json`.
- **Degradé (un atardecer):** en los bordes exteriores, naranjas cálidos (tacao `#FAC29A`, rosebud `#FAB795`, apricot `#F09483`). Al medio, cranberry `#E95678` y magenta `#EE64AC`. En el centro, lavender `#B877DB`.
- **Cómo activarlo:** LOWER + RAISE + **P**.
- **Consumo:** 69% de carga (es el más claro después de Catppuccin), atenuado al ~73% (`HORIZON_POWER_SCALE` `185`).

---

## 8. Cómo modificar los valores

### 8.1 Flujo de trabajo

1. Editá el archivo `~/bastardkb-qmk/keyboards/bastardkb/dilemma/4x6_4_procyon/keymaps/mac/config.h`.
2. Compilá en una terminal:

```bash
export PATH="$HOME/.local/bin:/opt/homebrew/opt/arm-none-eabi-gcc@8/bin:/opt/homebrew/opt/arm-none-eabi-binutils/bin:/opt/homebrew/opt/make/libexec/gnubin:$PATH"
cd ~/bastardkb-qmk
qmk compile -kb bastardkb/dilemma/4x6_4_procyon -km mac
```

3. El archivo nuevo queda en `~/bastardkb-qmk/bastardkb_dilemma_4x6_4_procyon_mac.uf2`.
4. Flasheá **las dos mitades** (sección 3).

> Las distancias están en **centésimas de milímetro**: `100` = 1 mm.

### 8.2 Tabla de ajustes

#### Cursor

| Problema | Parámetro | Actual | Qué hacer |
|---|---|---|---|
| El cursor arranca muy lento o muy rápido | `MAC_CURSOR_CPI_DEFAULT` | `100` (25%) | `400` = 100%. Usá un valor de la lista: 50, 75, 100, 150, 200, 300, 400, 600 u 800. |

Si cambiás `MAC_CURSOR_CPI_DEFAULT`, el valor nuevo solo se aplica después de borrar la memoria con RAISE + `EE_CLR`, o subiendo el número `USER_CONFIG_MAGIC` en `keymap.c`. Para el uso diario es más simple usar las teclas de velocidad.

#### Click y scroll

| Problema | Parámetro | Actual | Qué hacer |
|---|---|---|---|
| El toque no hace click | `DIGITIZER_MOUSE_TAP_DISTANCE` | `50` (0.5 mm) | Subilo a `80`–`100` |
| Hace clicks sin querer | `DIGITIZER_MOUSE_TAP_DETECTION_TIMEOUT` | `200` ms | Bajalo a `150` |
| El scroll va muy rápido | `DIGITIZER_SCROLL_DIVISOR` | `150` (1 paso cada 1.5 mm) | Subilo a `200`–`250` |
| El scroll va muy lento | `DIGITIZER_SCROLL_DIVISOR` | `150` | Bajalo a `80`–`100` |
| El scroll **horizontal** va muy rápido o muy lento | `DIGITIZER_SCROLL_DIVISOR_H` | `100` (1 paso por mm) | Subilo para más lento, bajalo para más rápido |

#### Zoom

| Problema | Parámetro | Actual | Qué hacer |
|---|---|---|---|
| El zoom va muy rápido | `DIGITIZER_PINCH_DIVISOR` | `250` (1 paso cada 2.5 mm) | Subilo a `350`–`400` (bajalo a `150` si queda muy lento) |
| El scroll se confunde con zoom | `DIGITIZER_PINCH_OPPOSITE_COS_PERCENT` | `50` | Subilo a `70`: exige que los dedos vayan más opuestos |
| Al zoom le cuesta activarse | `DIGITIZER_PINCH_OPPOSITE_COS_PERCENT` | `50` | Bajalo a `30` |
| Tarda en decidir entre scroll y zoom | `DIGITIZER_TWO_FINGER_DECISION_DISTANCE` | `120` (1.2 mm) | Bajalo a `80` |
| El scroll pega un salto al arrancar | `DIGITIZER_TWO_FINGER_DECISION_DISTANCE` | `120` | Bajalo a `80` |

#### Atrás/adelante en cualquier parte

| Problema | Parámetro | Actual | Qué hacer |
|---|---|---|---|
| Se va para atrás sin querer al hacer scroll horizontal | `DIGITIZER_FLICK_DISTANCE` / `DIGITIZER_FLICK_TIMEOUT` | `600` / `100` ms | Subí la distancia a `800`, o bajá el tiempo a `80` |
| Cuesta que dispare atrás/adelante | `DIGITIZER_FLICK_DISTANCE` / `DIGITIZER_FLICK_TIMEOUT` | `600` / `100` ms | Bajá la distancia a `450`, o subí el tiempo a `130` |
| Lo dispara aunque el movimiento sea algo diagonal | `DIGITIZER_FLICK_THRESHOLD` | `250` (2.5 mm) | Bajalo a `150` |

#### Gestos de borde

| Problema | Parámetro | Actual | Qué hacer |
|---|---|---|---|
| Cuesta activar los gestos de borde | `DIGITIZER_EDGE_SWIPE_ZONE` | 25% (10.5 mm) | Cambiá `25` por `30` en la fórmula |
| Los gestos de borde se activan sin querer (scroll lateral que termina en atrás) | `DIGITIZER_EDGE_SWIPE_ZONE` | 25% | Bajalo a `20` |
| Hay que deslizar demasiado desde el borde | `DIGITIZER_EDGE_SWIPE_DISTANCE` | `900` (9 mm) | Bajalo a `600` |

#### Swipes de 3 dedos

| Problema | Parámetro | Actual | Qué hacer |
|---|---|---|---|
| Hay que deslizar mucho | `DIGITIZER_MOUSE_SWIPE_DISTANCE` | `500` (5 mm) | Bajalo a `350` |
| El swipe sale torcido y no se activa | `DIGITIZER_MOUSE_SWIPE_THRESHOLD` | `300` (3 mm) | Subilo a `400` |

#### Luces

El brillo por defecto está en `config.h`.

| Problema | Parámetro | Actual | Qué hacer |
|---|---|---|---|
| Brillo por defecto | `RGB_MATRIX_DEFAULT_VAL` | `64` | No lo subas mucho: con las dos mitades conectadas, mucho brillo hace caer el voltaje (ver sección 10). |
| Atenuación del tema Catppuccin | `CATPPUCCIN_POWER_SCALE` | `160` (63%) | Hace que el tema consuma como un color saturado. No lo subas. |

La onda se configura en `rgb_matrix_user.inc`:

| Problema | Parámetro | Actual | Qué hacer |
|---|---|---|---|
| La onda oscurece demasiado o muy poco | `CATPPUCCIN_RIPPLE_DIM` | `210` (~82% más oscuro) | Bajalo a `140` para una onda más suave; el máximo es `255` (apaga las teclas en el anillo) |
| La onda viaja muy rápido o muy lento | `CATPPUCCIN_RIPPLE_DURATION` | `1300` ms | Subilo para que viaje más lento, bajalo para más rápido |
| La onda no cruza todo el teclado, o llega demasiado lejos | `CATPPUCCIN_RIPPLE_RADIUS` | `224` (todo el ancho) | Bajalo a `120` para que se quede cerca de la tecla |
| El anillo es muy fino o muy grueso | `CATPPUCCIN_RIPPLE_WIDTH` | `28` (~1.5 teclas) | Ajustá entre `18` y `40` |

> Si quitás o volvés a poner `#define RGB_MATRIX_KEYPRESSES` (la onda), subí también `USER_CONFIG_MAGIC` en `keymap.c`. Ese define cambia la posición del tema en la lista de efectos, y sin el cambio de versión el teclado mostraría otro efecto.

### 8.3 Cambiar qué hace cada gesto

Los atajos están en `config.h` como `DIGITIZER_..._KC`. Se escriben con modificadores de QMK:

| Modificador de QMK | Tecla en el Mac |
|---|---|
| `G(...)` | `Cmd` |
| `C(...)` | `Ctrl` |
| `A(...)` | `Opt` |
| `S(...)` | `Shift` |

Ejemplos:
- `G(KC_TAB)` = `Cmd+Tab`.
- `C(A(G(KC_N)))` = `Ctrl+Opt+Cmd+N`.

### 8.4 Respaldar y agregar capas

Tu configuración vive en dos lugares, y cada uno se respalda distinto:

| Qué | Dónde vive | Cómo se respalda o se restaura |
|---|---|---|
| Gestos, tema, velocidades y layout de fábrica | El firmware (`.uf2`) | Hay copias en `~/Downloads/dilemma/respaldo/`. Flasheá la versión que quieras recuperar. |
| Lo que cambies en VIA (teclas, capas, color, brillo) | La memoria del teclado | VIA → pestaña **SAVE + LOAD** → **Save** baja un `.json`; **Load** lo vuelve a cargar. |

- **Capas:** VIA tiene **8 capas**:
  - las 3 primeras son las de fábrica (base, LOWER y RAISE);
  - de la 4 a la 7 arrancan vacías (transparentes);
  - la 8 tiene los temas y las luces, y se activa con LOWER + RAISE.

  Para usar una capa libre, asigná en VIA una tecla `MO(3)` … `MO(6)` (la capa funciona mientras la mantenés apretada) o `TG(n)` (prende o apaga la capa).
- **Volver al tema después de cambiar colores:** LOWER + **T**. No borra nada más.
- **Volver a todo lo de fábrica:** LOWER + **V** (`EE_CLR`). Después cargá tu `.json` de VIA si tenías un layout propio.
- **Si cambiás el código:** guardá cada `.uf2` nuevo con fecha en `respaldo/` antes de flashearlo.

---

## 9. Solución de problemas

| Síntoma | Causa probable | Solución |
|---|---|---|
| Solo funciona la mitad que tiene el USB | Problema con el TRRS o con el cable puente | Revisá con un multímetro la continuidad del cable puente (pad `S` ↔ pad del IO) en ambas mitades |
| Los gestos y el zoom no hacen nada, pero el cursor sí | El USB está en la mitad sin trackpad | Conectá el USB a la mitad derecha |
| Las teclas de velocidad no hacen nada | VIA conserva un layout viejo | RAISE + `EE_CLR`, o reasignalas en VIA con `0x7E40`/`0x7E41` |
| El trackpad deja de responder y las luces se ven rojas o naranjas | Caída de voltaje por exceso de brillo | Bajá el brillo en VIA. El firmware reconfigura el trackpad solo; si no vuelve, desenchufá y volvé a enchufar el USB |
| VIA no detecta el teclado | El navegador no es compatible | Usá Chrome o Edge (Safari no soporta WebHID) |
| El teclado no entra en modo de carga | — | Usá el botón interno con el USB conectado (método de recuperación) |

---

## 10. Limitaciones de macOS

macOS no soporta trackpads USB externos con gestos nativos. Por eso:

- **No hay inercia en el scroll** ni gestos que "acompañan" al dedo, como Mission Control deslizándose.
- **Cada gesto dispara un atajo de una sola vez.**
- **El teclado no sabe dónde está el puntero en la pantalla.** Los gestos dependen solo de la posición de los dedos en el trackpad.
- **Si presionás una tecla durante un zoom**, se suelta el `Cmd` y el resto del gesto pasa a ser scroll normal.

### Consumo eléctrico de las luces

Con las dos mitades conectadas, las dos se alimentan del mismo USB, que pasa por el cable TRRS: son 104 LEDs. Los pasteles de Catppuccin tienen los tres canales (rojo, verde y azul) prendidos fuerte, con una carga promedio del **79%** por LED. Un color saturado de VIA ronda el **33–67%**. Con brillo alto, el voltaje cae:

- los LEDs pierden azul y verde y se ven **rojos o naranjas**;
- el chip del trackpad se reinicia.

Por eso el tema se atenúa (`CATPPUCCIN_POWER_SCALE`) y el brillo por defecto es bajo. Si el chip del trackpad se reinicia igual, el firmware ahora lo reconfigura solo.

---

## 11. Historial de cambios (rama `feat/mac-trackpad-gestures`)

| Commit | Descripción |
|---|---|
| `bf1b539d` | Pin serial a `GP16` |
| `59599229` | Swipes con atajos de 16 bits + keymap `mac` |
| `e041775d` | Velocidad del cursor con teclas |
| `8e29651b` | Zoom con dos dedos |
| `a5fe9e81` | Velocidad por defecto 25% + recarga del layout de VIA |
| `7b8c8116` | Diferenciar zoom y scroll por dirección |
| `59bd7099` | Tema Catppuccin Mocha con onda tenue |
| `fcc20f9d` | Gestos desde el borde |
| `5a19ff65` | Borde derecho → Centro de Notificaciones |
| `6b891372` | Movimiento rápido con dos dedos → atrás/adelante |
| `16481eb3` | Se quita la onda (en ese momento se creyó que rompía el trackpad; la causa real era el consumo); scroll y zoom menos sensibles |
| `eee05cf1` | Se vuelve a aplicar el tema Catppuccin al arrancar |
| `5917c025` | Atrás/adelante como botones del mouse (compatibles con teclado en español); scroll y zoom mucho menos sensibles; zona de borde al 25% |
| `23f99772` | El trackpad se reconfigura solo si su chip se reinicia |
| `660d4ed2` | Zonas de borde opcionales y velocidad separada para el scroll horizontal |
| `2a6ac9a5` | Vuelve la onda tenue al presionar teclas |
| `e140aa68` | La onda pasa a ser un anillo que oscurece y cruza todo el teclado |
| `c91ecf3b` | Onda más oscura (82%) y más lenta (1.3 s) |
| `3f78f78e` | Tecla LOWER + T para volver al tema; 8 capas en VIA |
| `8a40b57e` | Tema alternativo Gruvbox Dark (LOWER + Tab desde Catppuccin) |
| `e1e84081` | Capa 8 de temas y luces (LOWER + RAISE); brillo máximo limitado a 128 |
| `67e21a56` | Tema Alegra (LOWER + RAISE + I); prender/apagar luces pasa a K |
| `5fdf859c` | Temas Kanagawa Dragon (O) y Horizon (P) en la capa 8; el tono pasa a 9/0 |
| `7131d727` | Tema Alegra: Índigo solo en los bordes y Teal predominante |
| `da45894c` | Tema Desierto reemplaza a Kanagawa Dragon (tecla O) |
| `126f13ad` | Catppuccin atenuado para consumir menos, brillo por defecto 64, se quita el gesto de notificaciones, atrás/adelante más rápido (100 ms), scroll horizontal más rápido |

Los cambios están solo en tu computadora: no se subieron a ningún repositorio remoto.
