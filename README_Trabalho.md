# Portabilidade Ot-Rtos para placa SAM-r21


## OpenThread Network Simulator (OTNS)

### 1. Instalação do Go:
O Go pode ser instalado pelo link
[https://golang.org/dl/](https://golang.org/dl/)

### 2. Openthread network simulator:
Siga até o passo 3 
[https://openthread.io/codelabs/openthread-network-simulator#0](https://openthread.io/codelabs/openthread-network-simulator#0)

### 3. Criar openthread para OTNS:
A partir do passo 3 do [https://openthread.io/codelabs/openthread-network-simulator?hl=pt-br#2](https://openthread.io/codelabs/openthread-network-simulator?hl=pt-br#2)

Para compilar o Openthread, utilizar, também a flag “-DOT_OTNS=1” para que o OTNS detecte a instalação do Openthread na máquina.
```bash
$ ./script/cmake-build simulation -DOT_OTNS=ON -DOT_SIMULATION_VIRTUAL_TIME=ON -DOT_SIMULATION_VIRTUAL_TIME_UART=ON -DOT_SIMULATION_MAX_NETWORK_SIZE=999. 
```
### 4. Após a clonagem do repositório OTNS:
```bash
$ install-dep
$ install
```

### 5. Rode o OTNS no terminal da pasta 
/src/openthread/build/simulation/examples/apps/cli

## Compilação e Flash do OpenThread para a Placa Atmel SAMR21

### 6. Baixar GNU Toolchain:
```bash
$ cd <path-to-ot-samr21>
$ ./script/bootstrap
```

### 7. Build:
```bash
$ cd <path-to-ot-samr21>
$ ./script/build
```

### 8. Flash Binary:
```bash
Após o build bem-sucedido:
$ openocd -f board/atmel_samr21_xplained_pro.cfg
Em outro terminal:
$ cd <path-to-ot-samr21>/build/bin
$ arm-none-eabi-gdb ot-cli-ftd
$ (gdb) target remote 127.0.0.1:3333
$ (gdb) load
$ (gdb) monitor reset
$ (gdb) c
```

### 9. Formar uma Rede Thread no Thread Border Router::
Siga a partir do Passo 4
- [openthread.io/codelabs/esp-openthread-hardware#2](https://openthread.io/codelabs/esp-openthread-hardware?hl=pt-br#3)

## Compilação do OpenThread com RTOS no Sistema Linux

### 10. Linux Simulation:
```bash
$ git submodule update --init
$ mkdir build && cd build
$ cmake .. -DPLATFORM_NAME=linux
$ make -j12
Isso compilará a aplicação de teste CLI em build/ot_cli_linux.
```

## Compilação do OpenThread com RTOS na Placa Nordic nRF52840

### 11. Nordic Simulation:
```bash
$ git submodule update --init
$ mkdir build && cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.cmake -DPLATFORM_NAME=nrf52
$ make -j12
Isso construirá a aplicação de teste CLI em build/ot_cli_nrf52840.hex.
```

### 12. Configurar o RCP Joiner:
```bash
$ cd ~/src
$ git clone --recursive https://github.com/openthread/ot-nrf528xx.git
$ cd ot-nrf528xx
$ script/build nrf52840 USB_trans
```

## Compilação do Código para a Placa SAMR21

### 13. Build:
```bash
$ git submodule update --init
$ mkdir build && cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.cmake -DPLATFORM_NAME=samr21
$ make -j12
```

## Modificação no CMakeLists.txt da Raíz do OT-RTOS

### 14. Adicionar Subpasta "microchip" a pasta "Third Party" do SAMR21:
Coloque a subpasta "microchip" dentro de "third_party" no diretório do SAMR21 para o third party do OT-RTOS.

### 15. No arquivo CMakeLists.txt, substitua o bloco condicional "if" da plataforma Nordic pelo seguinte para a plataforma SAMR21:
```bash
if (${PLATFORM_NAME} STREQUAL samr21)
    add_executable(ot_demo_101
        ${SRC_DIR}/apps/cli/main.c
        ${SRC_DIR}/core/io_redirect.c
    )

    target_link_libraries(ot_demo_101
        PUBLIC
            demo_101
    )

    target_compile_definitions(ot_demo_101
        PUBLIC
            __HEAP_SIZE=8192
            __STACK_SIZE=8192
    )

    target_compile_definitions(ot_cli_${PLATFORM_NAME}
        PUBLIC
            __HEAP_SIZE=8192
            __STACK_SIZE=8192
    )

    #special link script
    set_target_properties(ot_cli_${PLATFORM_NAME} PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/openthread/repo/microship/include/samr21x18a.ld)
    set_target_properties(ot_cli_${PLATFORM_NAME} PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/third_party/openthread/repo/microship/include/samr21x18a.ld -lc -lnosys -lm -lstdc++")
    set_target_properties(ot_demo_101 PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/openthread/repo/microship/include/samr21x18a.ld)
    set_target_properties(ot_demo_101 PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/third_party/openthread/repo/microship/include/samr21x18a.ld -lc -lnosys -lm -lstdc++")

    #build hex file
    add_custom_command(OUTPUT ot_cli_${PLATFORM_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O ihex ot_cli_${PLATFORM_NAME} ot_cli_${PLATFORM_NAME}.hex
        DEPENDS ot_cli_${PLATFORM_NAME}
    )
    add_custom_target(ot_cli_nrf52_hex ALL DEPENDS ot_cli_${PLATFORM_NAME}.hex)

    add_custom_command(OUTPUT ot_demo_101.hex
        COMMAND arm-none-eabi-objcopy -O ihex ot_demo_101 ot_demo_101.hex
        DEPENDS ot_demo_101
    )
    add_custom_target(ot_demo_101_hex ALL DEPENDS ot_demo_101.hex)
endif()
```

### 16. Adicionar o FreeRTOSConfig.h

Adicione o arquivo FreeRTOSConfig.h da pasta "freertos_portable" do OT-RTOS a pasta "third-party".

### 17. Adicionar o Arquivo cli.h

Inclua o arquivo cli.h da pasta "openthread" do Openthred a pasta "third-party".
