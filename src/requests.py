from pysnmp.hlapi import *
import time

def snmpget(ip, oid):
    '''snmpget -v 1 -c public ip oid'''
    g = getCmd(SnmpEngine(),
               CommunityData('public', mpModel=0),
               UdpTransportTarget((ip, 161)),
               ContextData(),
               ObjectType(ObjectIdentity(oid)))
    errorIndication, errorStatus, errorIndex, varBinds = next(g)
    if errorIndication:
        print(errorIndication)
    else:
        if errorStatus:
            print('%s at %s' % (
                errorStatus.prettyPrint(),
                errorIndex and varBinds[int(errorIndex)-1] or '?'))
        else:
            for name, val in varBinds:
                return val/100  # int->float


def get_temperature(sensor, ip):
    oid = '.1.3.6.1.4.1.5.'+str(sensor)
    return snmpget(ip, oid)


def test_temperatures():
    ip = '192.168.0.46'
    nb_sensors = 16
    sensors = [i for i in range(1,nb_sensors+1)]
    t0 = time.time()
    for s in sensors:
        print("Sensor", s," : ", get_temperature(s,ip), "°C")
    #print("Delay for ", nb_sensors, " sensors : ", round(time.time()-t0,2), "s")
    

if __name__ == "__main__":
    test_temperatures()
   