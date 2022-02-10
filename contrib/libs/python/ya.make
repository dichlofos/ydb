OWNER(
    spreis
    orivej
    g:contrib
)

PY23_LIBRARY()

LICENSE(YandexOpen)

LICENSE_TEXTS(.yandex_meta/licenses.list.txt)

NO_PYTHON_INCLUDES()

IF (USE_ARCADIA_PYTHON)
    PEERDIR(
        contrib/libs/python/Include
        library/python/symbols/module
        library/python/symbols/libc
        library/python/symbols/python
    )
    IF (NOT OS_WINDOWS AND NOT OPENSOURCE)
        PEERDIR( 
            library/python/symbols/uuid 
        ) 
    ENDIF() 
    IF (MODULE_TAG == "PY2")
        PEERDIR(
            contrib/tools/python/lib
            library/python/runtime
        )
    ELSE()
        PEERDIR(
            contrib/tools/python3/lib
            library/python/runtime_py3
        )
    ENDIF()
ELSE()
    IF (USE_SYSTEM_PYTHON)
        PEERDIR(
            build/platform/python
        )
    ELSE()
        CFLAGS(GLOBAL $PYTHON_INCLUDE)
    ENDIF()
ENDIF()

END()

RECURSE(
    Include
)
