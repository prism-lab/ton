cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

set(SMC_ENVELOPE_SOURCE
        smc-envelope/GenericAccount.cpp
        smc-envelope/HighloadWallet.cpp
        smc-envelope/HighloadWalletV2.cpp
        smc-envelope/ManualDns.cpp
        smc-envelope/MultisigWallet.cpp
        smc-envelope/PaymentChannel.cpp
        smc-envelope/SmartContract.cpp
        smc-envelope/SmartContractCode.cpp
        smc-envelope/WalletInterface.cpp
        smc-envelope/WalletV3.cpp

        smc-envelope/GenericAccount.h
        smc-envelope/HighloadWallet.h
        smc-envelope/HighloadWalletV2.h
        smc-envelope/ManualDns.h
        smc-envelope/MultisigWallet.h
        smc-envelope/SmartContract.h
        smc-envelope/SmartContractCode.h
        smc-envelope/WalletInterface.h
        smc-envelope/WalletV3.h
)

file(MAKE_DIRECTORY smartcont/auto)
if (NOT CMAKE_CROSSCOMPILING)
    add_custom_target(gen_fif ALL)
    function(GenFif)
        set(options )
        set(oneValueArgs DEST NAME)
        set(multiValueArgs SOURCE)
        set(FUNC_LIB_SOURCE smartcont/stdlib.fc)
        cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
        string(REGEX REPLACE "[^0-9a-zA-Z_]" "_" ID ${ARG_DEST})
        set(ARG_DEST_FIF "${ARG_DEST}.fif")
        add_custom_command(
                COMMENT "Generate ${ARG_DEST_FIF}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMAND func -PS -o ${ARG_DEST_FIF} ${FUNC_LIB_SOURCE} ${ARG_SOURCE}
                MAIN_DEPENDENCY ${ARG_SOURCE}
                DEPENDS func ${FUNC_LIB_SOURCE}
                OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_DEST_FIF}
        )
        set(ARG_DEST_CPP "${ARG_DEST}.cpp")
        add_custom_command(
                COMMENT "Generate ${ARG_DEST_CPP}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMAND fift -Ifift/lib:smartcont -s asm-to-cpp.fif ${ARG_DEST_FIF} ${ARG_DEST_CPP} ${ARG_NAME}
                MAIN_DEPENDENCY ${ARG_SOURCE}
                DEPENDS fift ${ARG_DEST_FIF} smartcont/asm-to-cpp.fif fift/lib/Asm.fif
                OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_DEST_CPP}
        )
        add_custom_target(gen_fif_${ID} DEPENDS ${ARG_DEST_FIF} ${ARG_DEST_CPP})
        add_dependencies(gen_fif gen_fif_${ID})
    endfunction()

    GenFif(DEST smartcont/auto/config-code SOURCE smartcont/config-code.fc NAME config)
    GenFif(DEST smartcont/auto/wallet-code SOURCE smartcont/wallet-code.fc NAME wallet)
    GenFif(DEST smartcont/auto/wallet3-code SOURCE smartcont/wallet3-code.fc NAME wallet3)
    GenFif(DEST smartcont/auto/simple-wallet-code SOURCE smartcont/simple-wallet-code.fc NAME simple-wallet)
    GenFif(DEST smartcont/auto/highload-wallet-code SOURCE smartcont/highload-wallet-code.fc NAME highload-wallet)
    GenFif(DEST smartcont/auto/highload-wallet-v2-code SOURCE smartcont/highload-wallet-v2-code.fc NAME highload-wallet-v2)
    GenFif(DEST smartcont/auto/elector-code SOURCE smartcont/elector-code.fc NAME elector-code)
    GenFif(DEST smartcont/auto/multisig-code SOURCE smartcont/multisig-code.fc NAME multisig)
    GenFif(DEST smartcont/auto/restricted-wallet-code SOURCE smartcont/restricted-wallet-code.fc NAME restricted-wallet)
    GenFif(DEST smartcont/auto/restricted-wallet2-code SOURCE smartcont/restricted-wallet2-code.fc NAME restricted-wallet2)
    GenFif(DEST smartcont/auto/restricted-wallet3-code SOURCE smartcont/restricted-wallet3-code.fc NAME restricted-wallet3)
    GenFif(DEST smartcont/auto/pow-testgiver-code SOURCE smartcont/pow-testgiver-code.fc NAME pow-testgiver)

    GenFif(DEST smartcont/auto/dns-manual-code SOURCE smartcont/dns-manual-code.fc NAME dns-manual)
    GenFif(DEST smartcont/auto/dns-auto-code SOURCE smartcont/dns-auto-code.fc NAME dns-auto)

    GenFif(DEST smartcont/auto/payment-channel-code SOURCE smartcont/payment-channel-code.fc NAME payment-channel)

    GenFif(DEST smartcont/auto/simple-wallet-ext-code SOURCE smartcont/simple-wallet-ext-code.fc NAME simple-wallet-ext)
endif()

add_library(smc-envelope ${SMC_ENVELOPE_SOURCE})
target_include_directories(smc-envelope PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(smc-envelope PUBLIC ton_crypto PRIVATE tdutils ton_block)
if (NOT CMAKE_CROSSCOMPILING)
    add_dependencies(smc-envelope gen_fif)
endif()


set(SMARTCONT_TEST_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/test-smartcont.cpp
        PARENT_SCOPE
)
