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
| 10 | Layout de Gentleman y capa Herdr | QWERTY con home-row mods y las capas `MAGIC`, `PROG`, `NUMBERS`, `NAV`, `POINTER` y `THEME` del keymap `vendor` de Gentleman, más una capa que manda los atajos de Herdr con una sola mano |

---

## 2. Archivos importantes

| Qué | Dónde |
|---|---|
| Firmware listo para flashear | `~/Downloads/dilemma/dilemma_max_mac_catppuccin.uf2` |
| Código fuente del firmware | `~/bastardkb-qmk` (rama `feat/mac-trackpad-gestures`) |
| **Configuración de gestos (el archivo que vas a editar)** | `~/bastardkb-qmk/keyboards/bastardkb/dilemma/4x6_4_procyon/keymaps/mac/config.h` |
| Teclas personalizadas y capas | `.../keymaps/mac/keymap.c` |
| Efecto de luces Catppuccin | `.../keymaps/mac/rgb_matrix_user.inc` |
| Configuración de Herdr (el prefijo tiene que coincidir con el del teclado) | `~/.config/herdr/config.toml` |
| Documentación técnica (inglés) | `~/Downloads/dilemma/trackpad-mac-gestures-readme.md` |
| Registro de tareas | `~/Downloads/dilemma/odd/tasks/mac-trackpad-gestures.md` |

---

## 3. Cómo flashear el firmware

Cada mitad tiene su propio chip, así que **siempre se flashean por separado**, una a la vez y con el mismo archivo `.uf2`.

### Método recomendado: con teclas y sin tocar el TRRS

El TRRS **se queda conectado todo el tiempo**: solo movés el cable USB. Flashear con las dos mitades conectadas no es un problema (lo confirmó Quentin, de Bastard KB). Lo peligroso es conectar o desconectar el TRRS con el USB enchufado, y en este método no lo tocás nunca.

`QK_BOOT` siempre reinicia en modo de carga **la mitad que tiene el USB**, aunque apretes la tecla en la otra mitad.

1. **USB en la mitad derecha.** Mantené **`FN` derecho** (`MAGIC`, el pulgar derecho más cercano al centro) y apretá **`Tab`**. También sirve la tecla de `\` (columna exterior derecha, fila de la Q).
2. Aparece la unidad **`RPI-RP2`**. Arrastrá el `.uf2` a esa unidad. La mitad se reinicia sola.
3. **Desenchufá el USB de la derecha y enchufalo en la izquierda.** El TRRS sigue conectado.
4. Mantené otra vez **`FN` derecho** y apretá **`Tab`**. Ahora entra en modo de carga la izquierda. Arrastrá el `.uf2`.
5. Volvé a pasar el USB a la **mitad derecha**, la del trackpad.

> **Si una mitad todavía tiene el firmware anterior al layout de Gentleman**, `QK_BOOT` está en LOWER + **B** (izquierda) o RAISE + **N** (derecha).
>
> **Cuidado con la tecla de abajo:** justo debajo de `Tab` y de `\`, en la columna exterior de la fila de la A (donde están `Esc` y `'`), está `EE_CLR`. Con `FN` derecho, esa tecla borra los ajustes guardados (layout de VIA, velocidad del cursor, modo de zoom y luces).

### Método alternativo: botón de reset

Si una mitad no responde (por ejemplo, porque tiene un firmware roto), `QK_BOOT` no funciona. En ese caso, conectá el USB a esa mitad y hacé **doble toque rápido en el botón de reset**. Después arrastrá el `.uf2` igual que antes.

**Regla de oro:** si alguna vez tenés que conectar o desconectar el TRRS, hacelo **siempre con el USB desenchufado**. Primero el TRRS y después el USB. Conectar o desconectar el TRRS con el USB enchufado fue lo que dañó el pin `GP1`.

> **Importante:** conectá el USB a la **mitad del trackpad** (la derecha). El cursor y el scroll funcionan desde cualquier lado. Los swipes, el zoom y los atajos solo llegan al Mac desde la mitad que tiene el USB.

### Primer arranque

La primera vez que arranques este firmware (versión de ajustes `0xDC`, la del layout de Gentleman) se **resetea una sola vez**:
- el layout de VIA,
- la velocidad del cursor,
- el modo de zoom,
- las luces.

Es intencional: VIA solo resetea el layout cuando cambia la *fecha* de compilación (`quantum/via.c:71-74`). Por eso, un firmware anterior compilado el mismo día dejó las teclas nuevas sin asignar. Con el cambio de versión (`USER_CONFIG_MAGIC` en `keymap.c`), el teclado carga las 8 capas nuevas aunque tuvieras un layout propio en VIA. Ojo: un `.json` de VIA guardado antes de este cambio trae el layout viejo, y si lo cargás pisa el nuevo.

---

## 4. Ajustes de macOS

### 4.1 Fuente de entrada: inglés (US)

El layout de Gentleman supone que macOS usa una fuente de entrada **en inglés (US)**. El teclado manda códigos de tecla, y es macOS el que decide qué símbolo sale: con la distribución en español, la capa de símbolos (`PROG`) y la puntuación de la capa Herdr mandan otros caracteres (por ejemplo, `@`, `{` o `[` salen cambiados).

Para cambiarla:

1. Abrí **Ajustes del Sistema → Teclado**.
2. En **Entrada de texto**, apretá **Editar…** al lado de **Fuentes de entrada**.
3. Apretá **+** y agregá **ABC** o **EE. UU.** (*U.S.*), que están en **Inglés**.
4. Quitá **Español** con **−**, o dejalo y cambiá de fuente con el menú de la barra superior o con `Ctrl+Espacio`. Si lo dejás, asegurate de estar en ABC/U.S. cuando uses el teclado.

Con la fuente en inglés, la **ñ** y los **acentos** salen con `Option`:

| Para escribir | Apretá |
|---|---|
| `ñ` | `Option+n` y después `n` |
| `á` `é` `í` `ó` `ú` | `Option+e` y después la vocal |
| `ü` | `Option+u` y después `u` |
| `¿` / `¡` | `Option+Shift+/` / `Option+1` |

> `Option` es `Alt`: en el teclado está en **S** o **L** mantenidas (home-row mods, sección 6.1) y en la columna exterior derecha de la fila de la Z (a la derecha de `/`).

### 4.2 Atajos que usa el firmware

No hace falta configurar nada. Estos atajos ya vienen activados por defecto en macOS:
- `Ctrl+flechas` para escritorios, Mission Control y App Exposé.
- Atrás y adelante se envían como **botones 4 y 5 del mouse** (no como `Cmd+[` / `Cmd+]`, que dependen de la distribución de teclado: en español se convierten en tilde y zoom). Funciona en Brave, Chrome, Arc, Edge y Firefox; no en Safari.

Si alguno no responde, revisá que esté activado en **Ajustes del Sistema → Teclado → Funciones rápidas de teclado**.

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

## 6. Capas y teclas especiales

El layout sigue el keymap `vendor` de Gentleman ([Alan-TheGentleman/qmk_userspace](https://github.com/Alan-TheGentleman/qmk_userspace)). Las posiciones se nombran por la tecla que hay ahí en la capa base: "la tecla donde está la Q". La **columna exterior** es la del borde de cada mitad: `Tab`, `Esc` y `` ` `` a la izquierda; `\`, `'` y `Alt` a la derecha.

**Pulgares**, de izquierda a derecha:

| Izquierda 1 | Izquierda 2 | Izquierda 3 | Izquierda 4 | Derecha 1 | Derecha 2 | Derecha 3 | Derecha 4 |
|---|---|---|---|---|---|---|---|
| Click izquierdo | `Space` | `PROG` | `NUMBS` | `MAGIC` | `Enter` | `Backspace` (`NAV` al mantener) | `HERDR` |

`NUMBS` y `MAGIC` son los dos pulgares más cercanos al centro.

| # | Capa | Cómo se activa | Qué tiene |
|---|---|---|---|
| 0 | `BASE` | Siempre | QWERTY con home-row mods |
| 1 | `MAGIC` | Mantené `MAGIC` | Luces, multimedia, `QK_BOOT` y `EE_CLR` |
| 2 | `PROG` | Mantené `PROG` | Símbolos de programación |
| 3 | `NUMBERS` | Mantené `NUMBS` | Números a la derecha, `F1`–`F9` a la izquierda |
| 4 | `NAV` | Mantené `Backspace` | Flechas, `Home`/`End`, `PgUp`/`PgDn`, `Caps Lock` |
| 5 | `POINTER` | Mantené **Z** o **/** | Botones del mouse, velocidad del cursor y modo de zoom |
| 6 | `HERDR` | Mantené `HERDR` | Atajos de Herdr |
| 7 | `THEME` | Mantené `NUMBS` + `MAGIC` a la vez | Temas y luces |

> Estas posiciones corresponden al layout por defecto. Si las movés en VIA, valen las que hayas asignado vos.

### 6.1 Capa base y home-row mods

Es QWERTY, con estas particularidades:

- Fila de números: `1`…`0`, y `-` en la columna exterior derecha. La columna exterior izquierda de esa fila no hace nada.
- Columna exterior izquierda: `Tab`, `Esc` y `` ` ``. Columna exterior derecha: `\`, `'` y `Alt`.
- **Z** y **/**, si las mantenés, activan `POINTER` (sección 6.6).

Las teclas de la fila del medio funcionan como modificador si las mantenés (*home-row mods*, en orden GACS):

| Tecla | Al mantenerla | Tiempo |
|---|---|---|
| **A** / **;** | `Cmd` | 260 ms |
| **S** / **L** | `Alt` (`Option`) | 260 ms |
| **D** / **K** | `Ctrl` | 260 ms |
| **F** / **J** | `Shift` | 200 ms |

El *tiempo* es cuánto tenés que mantener la tecla para que cuente como modificador. Está en `get_tapping_term` (`keymap.c`); el resto de las teclas de doble función (`⌫`/NAV, Z y / para POINTER) usa `TAPPING_TERM` (200 ms, en `config.h`).

Además está activado **Flow Tap** (`FLOW_TAP_TERM` 150 ms): si apretás una tecla de doble función menos de 150 ms después de la tecla anterior, **siempre escribe la letra**. Así, cuando escribís seguido, no se activan modificadores ni capas sin querer, aunque la mantengas un poco de más. Para usar el modificador, hacé una pausa corta antes.

#### Consejos para acostumbrarte

- **Para el modificador, mantené la tecla un poco más** y recién ahí apretá la otra. Por ejemplo, mantené **A** y apretá **C** para `Cmd+C`.
- **Para escribir, escribí normal**: soltá cada tecla rápido aunque las encadenes (*rolling*). Si dejás una tecla apretada de más, sale el modificador en vez de la letra.
- Para un atajo con una sola mano, usá el modificador de la otra mano: `Shift` con **J** para las letras de la izquierda y con **F** para las de la derecha.
- Si se disparan modificadores sin querer, subí el tiempo de esa tecla en `get_tapping_term` (`keymap.c`). Si te cuesta activarlos, bajalo. Después compilá y flasheá (sección 8.1).

### 6.2 MAGIC: luces, multimedia y modo de carga

Mantené **MAGIC** (el pulgar derecho más cercano al centro). Las dos mitades son espejo:

| Mitad izquierda | Mitad derecha | Función | Acción |
|---|---|---|---|
| Columna exterior, fila de la Q (`Tab`) | Columna exterior, fila de la Q (`\`) | `QK_BOOT` | Modo de carga de firmware (sección 3) |
| Columna exterior, fila de la A (`Esc`) | Columna exterior, fila de la A (`'`) | `EE_CLR` | Borra la memoria y vuelve a los valores por defecto |
| **T** | **Y** | `RM_TOGG` | Prende o apaga las luces |
| **G** | **H** | `RM_NEXT` | Siguiente efecto de luz |
| **R** / **F** | **U** / **J** | `RM_VALU` / `RM_VALD` | Sube / baja el brillo (tope 128) |
| **E** / **D** | **I** / **K** | `RM_HUEU` / `RM_HUED` | Tono (solo efectos de VIA, no los temas) |
| **W** / **S** | **O** / **L** | `RM_SATU` / `RM_SATD` | Saturación (solo efectos de VIA) |
| **Q** / **A** | **P** / **;** | `RM_SPDU` / `RM_SPDD` | Velocidad del efecto (solo efectos de VIA) |
| `` ` `` / **X** | Columna exterior, fila de la Z (`Alt`) / **.** | `KC_VOLU` / `KC_VOLD` | Sube / baja el volumen |
| **Z** | **/** | `KC_MUTE` | Silencio |
| **C** / **V** | **,** / **M** | `KC_MPRV` / `KC_MNXT` | Pista anterior / siguiente |
| Pulgar `Space` | Pulgar `Backspace` | `KC_MPLY` | Reproducir / pausar |
| Pulgar `PROG` | Pulgar `Enter` | `KC_MSTP` | Detener |

### 6.3 PROG: símbolos

Mantené **PROG** (pulgar izquierdo, el tercero desde la izquierda). Debajo de cada tecla de la base está lo que manda; `·` no hace nada. La fila de números y las columnas exteriores tampoco hacen nada.

```
Q    W    E    R    T        Y    U    I    O    P
·    `    ·    $    ·        ^    (    )    -    ·

A    S    D    F    G        H    J    K    L    ;
⇧    @    #    %    ·        &    {    }    =    '

Z    X    C    V    B        N    M    ,    .    /
`    ·    ·    ?    ·        *    [    ]    !    \
```

- `⇧` (en la **A**) es `Shift`: combinalo con los símbolos de la capa para sacar `~` (con `` ` ``), `"` (con `'`), `|` (con `\`), `_` (con `-`) o `+` (con `=`).
- Pulgares derechos: `Enter` sigue en `Enter`, `<` queda en `Backspace` y `>` en `HERDR`; `MAGIC` no hace nada. Los pulgares izquierdos quedan como en la base.

### 6.4 NUMBERS: números y teclas F

Mantené **NUMBS** (el pulgar izquierdo más cercano al centro). `·` funciona igual que en la base. La fila de números, las columnas exteriores y los pulgares también quedan como en la base.

```
Q    W    E    R    T        Y    U    I    O    P
·    ·    F7   F8   F9       ·    7    8    9    ·

A    S    D    F    G        H    J    K    L    ;
⇧    ⌥    F4   F5   F6       ·    4    5    6    0

Z    X    C    V    B        N    M    ,    .    /
⌘    ⌃    F1   F2   F3       ·    1    2    3    ·
```

`⇧` `⌥` `⌘` `⌃` son `Shift`, `Alt`, `Cmd` y `Ctrl` normales (sin home-row mods), para combinarlos con los números y las teclas F.

### 6.5 NAV: flechas y navegación

Mantené **Backspace** (pulgar derecho, el tercero desde la izquierda). Si lo tocás sin mantenerlo, borra como siempre.

| Tecla | Función |
|---|---|
| **H** / **J** / **K** / **L** | ← / ↓ / ↑ / → |
| **;** | `Caps Lock` |
| **N** | `Insert` |
| **M** / **/** | `Home` / `End` |
| **,** / **.** | `PgDn` / `PgUp` |
| **A** / **S** / **D** / **F** | `Cmd` / `Alt` / `Ctrl` / `Shift` normales, para `Shift`+flechas (seleccionar) o `Alt`+flechas (saltar palabras) |
| Pulgar `Enter` | `Enter` |

El resto de las teclas no hace nada en esta capa.

### 6.6 POINTER: mouse, velocidad del cursor y modo de zoom

Mantené **Z** con la mano izquierda (o **/** con la derecha) y usá el trackpad con la otra. Si la tocás sin mantenerla, escribe la letra.

| Tecla | Función | Acción |
|---|---|---|
| **2** / **3** | `CURSOR_SPEED_DOWN` / `CURSOR_SPEED_UP` | Cursor más lento / más rápido |
| **8** / **9** | `CURSOR_SPEED_UP` / `CURSOR_SPEED_DOWN` | Lo mismo, en espejo (como en el keymap de Gentleman) |
| **5** | `PINCH_MOD_CYCLE` | Cambia el modo de zoom |
| Pulgares click / `Space` / `PROG` | `MS_BTN2` / `MS_BTN1` / `MS_BTN3` | Click derecho / izquierdo / del medio |
| Pulgares `Enter` / `Backspace` / `HERDR` | `MS_BTN3` / `MS_BTN1` / `MS_BTN2` | Click del medio / izquierdo / derecho |
| **A S D F** y **J K L ;** | Modificadores | `Cmd` `Alt` `Ctrl` `Shift` y `Shift` `Ctrl` `Alt` `Cmd`, para `Cmd`+click o `Shift`+click |

**Velocidades del cursor** (de más lenta a más rápida):
12.5% → 19% → **25% (por defecto)** → 37% → 50% → 75% → 100% → 150% → 200%

**Modos de zoom** (cada toque de `PINCH_MOD_CYCLE` pasa al siguiente):
1. `Cmd` + scroll: **por defecto**, para Figma.
2. `Ctrl` + scroll.
3. Scroll solo, para AutoCAD.

La velocidad y el modo de zoom **se guardan** aunque desconectes el teclado.

### 6.7 HERDR: atajos de Herdr con una tecla

Herdr usa una tecla prefijo, como tmux. Mantené **HERDR** (el pulgar derecho exterior) y apretá una tecla: el teclado manda el prefijo `Ctrl+Q` (`HERDR_PREFIX`) y después esa tecla. Por ejemplo, **HERDR** + **C** es `prefijo c` (pestaña nueva).

- Las letras, los números y la puntuación están donde en la base, pero **sin home-row mods**.
- `[` (modo copia) está en la posición de `Esc`, y `?` (ayuda) a la derecha de `/` (columna exterior derecha de la fila de la Z).
- Para los atajos con `shift+…`, mantené también **Shift**, que está en el pulgar de `Space`. Para `alt+…`, mantené **Alt**, en el pulgar de `PROG`. El prefijo sale sin modificadores; el `Shift` o el `Alt` se aplican solo a la tecla.

Atajos por defecto de Herdr:

| Grupo | Acción | Herdr | Con HERDR mantenido, apretá |
|---|---|---|---|
| Pestañas | Nueva | `c` | **C** |
| | Siguiente / anterior | `n` / `p` | **N** / **P** |
| | Ir a la pestaña 1 a 9 | `1`…`9` | **1**…**9** |
| | Renombrar | `shift+t` | Shift + **T** |
| | Cerrar | `shift+x` | Shift + **X** |
| Paneles | Foco a la izquierda / abajo / arriba / derecha | `h` / `j` / `k` / `l` | **H** / **J** / **K** / **L** |
| | Intercambiar | `shift+h/j/k/l` | Shift + **H** / **J** / **K** / **L** |
| | Siguiente / anterior | `tab` / `shift+tab` | **Tab** / Shift + **Tab** |
| | Dividir vertical / horizontal | `v` / `-` | **V** / **-** |
| | Cerrar | `x` | **X** |
| | Zoom | `z` | **Z** |
| | Redimensionar | `r` | **R** |
| | Modo copia | `[` | **[** (posición de `Esc`) |
| | Atajos con Alt | `alt+j` / `alt+k` | Alt + **J** / **K** |
| Workspaces | Selector | `w` | **W** |
| | Nuevo | `shift+n` | Shift + **N** |
| Otros | Ir a (*goto*) | `g` | **G** |
| | Barra lateral | `b` | **B** |
| | Ajustes | `s` | **S** |
| | Ayuda | `?` | **?** (a la derecha de `/`) |
| | Desconectar (*detach*) | `q` | **Q** |
| | Recargar | `shift+r` | Shift + **R** |

**El prefijo tiene que coincidir con el de Herdr.** El teclado manda `Ctrl+Q`, que corresponde a `[keys] prefix = "ctrl+q"` en `~/.config/herdr/config.toml`. Si cambiás el prefijo en Herdr, cambialo también en el firmware:

1. En `config.h`, cambiá `HERDR_PREFIX`. Por ejemplo, para `ctrl+b`: `#define HERDR_PREFIX C(KC_B)`.
2. Compilá y flasheá las dos mitades (sección 8.1).

Los modificadores, las teclas de capa, los botones del mouse y las teclas personalizadas nunca mandan el prefijo.

### 6.8 THEME: temas y luces (NUMBS + MAGIC)

Mantené **los dos pulgares del centro a la vez** (`NUMBS` y `MAGIC`) y usá la mano derecha:

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

`RGB_THEME_RESTORE` **vuelve al tema Catppuccin** con el brillo por defecto, sin tocar tu layout ni la velocidad del cursor.

El brillo máximo quedó limitado a **128** (el teclado permitía 176), así ni las teclas ni VIA pueden volver a provocar la caída de voltaje.

### 6.9 Lo que no se copió del layout de Gentleman

- **La capa del LCD:** este teclado no tiene pantalla.
- **Las teclas de DPI, *sniping* y *drag-scroll*:** no existen en este firmware. En su lugar, la capa `POINTER` tiene las teclas de velocidad del cursor y de modo de zoom; las demás posiciones quedan vacías.
- **El scroll invertido:** rompería los gestos del trackpad (sección 5).

### 6.10 Códigos para VIA

**Si usás VIA** y querés mover las teclas personalizadas a otra posición, usá la tecla **"Any"** con estos códigos:

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
- **Cómo activarlo:** NUMBS + MAGIC + **U**. También, desde Catppuccin, el siguiente efecto (`RM_NEXT`: NUMBS + MAGIC + **M**, o MAGIC + **G**) pasa a Gruvbox.
- **Cómo volver a Catppuccin:** NUMBS + MAGIC + **Y**.
- **Consumo:** Gruvbox es más saturado que Catppuccin (58% de carga contra 79%), así que solo se atenúa al ~87% (`GRUVBOX_POWER_SCALE` `221`).

### Tema alternativo: Alegra

- **Colores:** el **Índigo** secundario `#514EA6` solo en los bordes exteriores, y el **Teal** primario `#30ABA9` en todo el resto de cada mitad, para que predomine. En los LEDs el índigo se ve como un azul fuerte. Tiene la misma onda que los demás temas.
- **Cómo activarlo:** NUMBS + MAGIC + **I**.
- **Consumo:** 49% de carga, ya igual que un color saturado, así que no se atenúa (`ALEGRA_POWER_SCALE` `255`).
- **Para ajustar la proporción:** en `rgb_matrix_user.inc`, la lista `alegra_palette` tiene 1 Índigo y 4 Teal. Agregar más Teal achica todavía más la franja índigo.

### Tema alternativo: Desierto

- **Origen:** una paleta propia con la misma forma que Horizon: un cañón de roca roja al atardecer. Reemplazó a Kanagawa Dragon.
- **Degradé (desde los bordes exteriores hacia el centro):**

  | Color | Hex |
  |---|---|
  | Arena dorada | `#FF5A00` |
  | Ámbar | `#FF4600` |
  | Duna naranja | `#FF3600` |
  | Naranja quemado | `#F22800` |
  | Terracota | `#DA1A00` |
  | Rojo cañón | `#B00C00` |

- **Colores ajustados para LED:** en los LEDs del teclado el verde brilla mucho más que el rojo. Por eso estos colores tienen mucho rojo, poco verde y nada de azul. En el teclado se ven arena, naranja y rojo; en pantalla, estos hex se ven más naranjas. La primera versión, con tonos "reales" de arena, se veía verdosa o azulada en los bordes.
- **Cómo activarlo:** NUMBS + MAGIC + **O**.
- **Consumo y brillo:** al ser colores saturados, consume solo 37%. Por eso, en vez de atenuarlo, el firmware lo **aclara** hasta consumir lo mismo que los demás temas (`DESERT_POWER_SCALE` `346`, más de 255 = más brillo).
- **Regla para que no se vea verde:** el verde debe quedar en más o menos **un tercio del rojo o menos** (por ejemplo `#FF5A00`). Con más verde, en estos LEDs el amarillo se ve verdoso.
- **Para cambiar colores:** editá la lista `desert_palette` en `rgb_matrix_user.inc`. Va del borde exterior al centro.

### Tema alternativo: Horizon

- **Origen:** la paleta oficial de [jolaleye/horizon-theme-vscode](https://github.com/jolaleye/horizon-theme-vscode), archivo `src/dark/globals.json`.
- **Degradé (un atardecer):** en los bordes exteriores, naranjas cálidos (tacao `#FAC29A`, rosebud `#FAB795`, apricot `#F09483`). Al medio, cranberry `#E95678` y magenta `#EE64AC`. En el centro, lavender `#B877DB`.
- **Cómo activarlo:** NUMBS + MAGIC + **P**.
- **Consumo:** 69% de carga (es el más claro después de Catppuccin), atenuado al ~73% (`HORIZON_POWER_SCALE` `185`).

### Colores por capa

Mientras mantenés una capa, las luces te muestran qué teclas usar:
- **Las teclas que hacen algo en esa capa** se encienden con el color de la capa.
- **La tecla que estás manteniendo** se ve en **blanco**.
- **El resto** se apaga.

Al soltar, vuelve el tema.

| Capa | Color | Cómo se activa |
|---|---|---|
| PROG (símbolos) | Morado | `=+` (pulgar izquierdo) |
| NUMBS (números y F) | Naranja | `FN` izquierdo |
| NAV (flechas) | Verde | `⌫` mantenido |
| POINTER (mouse) | Azul | Z o / mantenidas |
| MAGIC (luces, multimedia) | Amarillo | `FN` derecho |
| HERDR | Turquesa | `Ctrl` (pulgar derecho exterior) |
| THEME (temas) | Rosa | Los dos `FN` |

- **Para cambiar los colores:** editá la lista `layer_colors` en `keymap.c`. Usá colores con poco verde, porque en estos LEDs el verde domina.
- **Si cambiás teclas en VIA:** VIA solo guarda los cambios en la mitad conectada por USB, así que la otra mitad va a seguir iluminando las teclas del layout original.

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

Si cambiás `MAC_CURSOR_CPI_DEFAULT`, el valor nuevo solo se aplica después de borrar la memoria con `EE_CLR` (MAGIC + la tecla donde está `Esc`), o subiendo el número `USER_CONFIG_MAGIC` en `keymap.c`. Para el uso diario es más simple usar las teclas de velocidad.

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

#### Home-row mods y Herdr

| Problema | Parámetro | Actual | Qué hacer |
|---|---|---|---|
| Al escribir rápido salen modificadores en vez de letras | `get_tapping_term` en `keymap.c` | `Cmd` 260, `Shift` 200, `Alt`/`Ctrl` 260 ms (Flow Tap 150 ms) | Subí el tiempo de esa tecla de a 20 ms |
| Cuesta activar el modificador | `get_tapping_term` en `keymap.c` | Igual | Bajá el tiempo de esa tecla de a 20 ms |
| Cambiaste el prefijo de Herdr | `HERDR_PREFIX` en `config.h` | `C(KC_Q)` (`Ctrl+Q`) | Poné el mismo prefijo que en `~/.config/herdr/config.toml` (sección 6.7) |

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

### 8.4 Respaldar y cambiar capas

Tu configuración vive en dos lugares, y cada uno se respalda distinto:

| Qué | Dónde vive | Cómo se respalda o se restaura |
|---|---|---|
| Gestos, tema, velocidades y layout de fábrica | El firmware (`.uf2`) | Hay copias en `~/Downloads/dilemma/respaldo/`. Flasheá la versión que quieras recuperar. |
| Lo que cambies en VIA (teclas, capas, color, brillo) | La memoria del teclado | VIA → pestaña **SAVE + LOAD** → **Save** baja un `.json`; **Load** lo vuelve a cargar. |

- **Capas:** VIA tiene **8 capas** (numeradas del 0 al 7) y **todas están en uso**: `BASE` (0), `MAGIC` (1), `PROG` (2), `NUMBERS` (3), `NAV` (4), `POINTER` (5), `HERDR` (6) y `THEME` (7), que se activa con NUMBS + MAGIC (sección 6). Ya no quedan capas libres: para una capa propia, reemplazá una que no uses (por ejemplo `HERDR`) y cambiá en VIA la tecla que la activa. Para tener más de 8 hay que subir `DYNAMIC_KEYMAP_LAYER_COUNT` en `config.h`, y eso mueve los datos de VIA (hay que subir también `USER_CONFIG_MAGIC` en `keymap.c`).
- **Volver al tema después de cambiar colores:** NUMBS + MAGIC + **Y** (`RGB_THEME_RESTORE`). No borra nada más.
- **Volver a todo lo de fábrica:** MAGIC + la tecla donde está `Esc` (`EE_CLR`). Después cargá tu `.json` de VIA si tenías un layout propio (guardado con el layout de Gentleman: uno anterior trae las capas viejas).
- **Si cambiás el código:** guardá cada `.uf2` nuevo con fecha en `respaldo/` antes de flashearlo.

---

## 9. Solución de problemas

| Síntoma | Causa probable | Solución |
|---|---|---|
| Solo funciona la mitad que tiene el USB | Problema con el TRRS o con el cable puente | Revisá con un multímetro la continuidad del cable puente (pad `S` ↔ pad del IO) en ambas mitades |
| Los gestos y el zoom no hacen nada, pero el cursor sí | El USB está en la mitad sin trackpad | Conectá el USB a la mitad derecha |
| Las teclas de velocidad no hacen nada | VIA conserva un layout viejo | `EE_CLR` (MAGIC + la tecla donde está `Esc`), o reasignalas en VIA con `0x7E40`/`0x7E41` |
| El trackpad deja de responder y las luces se ven rojas o naranjas | Caída de voltaje por exceso de brillo | Bajá el brillo en VIA. El firmware reconfigura el trackpad solo; si no vuelve, desenchufá y volvé a enchufar el USB |
| VIA no detecta el teclado | El navegador no es compatible | Usá Chrome o Edge (Safari no soporta WebHID) |
| El teclado no entra en modo de carga | — | Usá el botón interno con el USB conectado (método de recuperación) |
| `QK_BOOT` no hace nada en la mitad izquierda | Con el TRRS desconectado, `MAGIC` (en la mitad derecha) no llega | Usá el doble toque en el botón de reset (sección 3) |
| Los símbolos salen cambiados (`@`, `{`, `[`, `'`…) | macOS usa la distribución en español | Cambiá la fuente de entrada a ABC o U.S. (sección 4.1) |
| Al escribir rápido salen `Cmd`, `Ctrl` o `Shift` en vez de letras, o al revés | Tiempos de los home-row mods | Ver consejos en la sección 6.1 y ajustá `get_tapping_term` (sección 8.2) |
| La capa HERDR no hace nada en Herdr | El prefijo del teclado no coincide con el de Herdr | Revisá que `HERDR_PREFIX` en `config.h` y `[keys] prefix` en `~/.config/herdr/config.toml` sean iguales (sección 6.7) |

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
| `14d946a0` | Desierto más saturado y ajustado para LED, con más brillo al mismo consumo |
| `7698c460` | Desierto con menos verde: bordes amarillo anaranjado en vez de verdosos |
| `126f13ad` | Catppuccin atenuado para consumir menos, brillo por defecto 64, se quita el gesto de notificaciones, atrás/adelante más rápido (100 ms), scroll horizontal más rápido |
| `b1dd10c7` | Layout de Gentleman (home-row mods, capas PROG/NUM/NAV/POINTER/MAGIC) y capa Herdr; temas en NUM + MAGIC |
| `34e0389e` | Colores por capa: al mantener una capa se iluminan solo sus teclas activas |
| `04892854` | Teclas de doble función más tolerantes (Shift 200 ms; Cmd, Alt y Ctrl 260 ms; resto 200 ms) con Flow Tap; los colores de capa aparecen a los 250 ms |

Los cambios están solo en tu computadora: no se subieron a ningún repositorio remoto.
