#!/usr/bin/python
# -*- coding: utf-8 -*-

import numpy as np
import time
import sys

import qi


class NaoCaller:
    """ A service which reads in shared memory and send positions to
    NAO motors
    """

    is_listening = True
    session = None
    services = {}
    SHM_MOTORS_FILE = "/run/shm/nao.motors-tf"
    MOTORS = {
        #"HeadPitch" : [0, 1],
        #"HeadYaw" : [-1, 1],

        "RShoulderRoll" : [-2, -0.3],
        "LShoulderRoll" : [0, 1],
        "RShoulderPitch": [-1, 2],
        "LShoulderPitch": [-1, 2],

        "RElbowRoll" : [0, 2],
        "LElbowRoll" : [-2, 0],
        "RElbowYaw" : [-2.5, 0.4],
        "LElbowYaw" : [-2.5, -0.4],

        "RWristYaw" : [-1.8, 1.8],
        "LWristYaw" : [-1.8, 1.8],

        #"RHand" : [0, 1],
        #"LHand" : [0, 1],
	# Mis en commentaire apres soucis de position initiale
        # Experimental
        # "RHipPitch" : [-2, 0],
        # "LHipPitch" : [-2, 0],
        # "LHipYawPitch" : [0, 1],
        # "RHipRoll" : [-1, 1],
        # "LHipRoll" : [-1, 1],
 	# "RKneePitch" : [0, 1],
        # "LKneePitch" : [0, 1],

        #"RAnklePitch" : [TODO],
        #"LAnklePitch" : [TODO],
        #"RAnkleRoll" : [TODO],
        #"LAnkleRoll" : [TODO]
    }

    MOTORS_ORDER = [
        "RShoulderRoll",
        "LShoulderRoll",
        "RShoulderPitch",
        "LShoulderPitch",
        "RElbowRoll",
        "LElbowRoll",
        "LElbowYaw",
        "RElbowYaw",
        "LWristYaw",
	"RWristYaw"

        #"LKneePitch", #plante apres 0.6 sur vue metre
        #"RKneePitch", #plante apres 0.6 sur vue metre
    ]

    def __init__(self, app):
        self.session = app.session
        self._load_services({
            "tts": "ALTextToSpeech",
            "motion": "ALMotion",
            "posture": "ALRobotPosture"
        })
        self._init_position()
        time.sleep(2) # To check initial pos


    def _load_services(self, services_map):
        for alias, service in services_map.iteritems():
            self.services[alias] = self.session.service(service)

    def _check_motor(self, name, val):
        return name in self.MOTORS and self.MOTORS[name][0] <= val <= self.MOTORS[name][1]

    def _unnormalize_pos(self, name, raw_pos):
        # Raw pos between 0 and 1

        pos_min = self.MOTORS[name][0]
        pos_max = self.MOTORS[name][1]

        return raw_pos * (pos_max - pos_min) + pos_min

    def _init_position(self):
        #self.services['motion'].wakeUp()
        #self.move('HeadYaw', 0, 0.25, False)
        #self.move('HeadPitch', 0, 0.25, False)
        #self.move('LHipYawPitch', 0, 0.25, False)
        #self.move('RHipPitch', 0.0, 0.25, False)
        #self.move('LHipPitch', 0.0, 0.25, False)
       #self.move('RHipRoll', 0.0, 0.25, False)
       #self.move('LHipRoll', 0.0, 0.25, False)
	#self.move('LKneePitch', 0, 0.25, False)
	#self.move('RKneePitch', 0, 0.25, False)

	names = 'Body'
	stiffness = 1.0
	self.services['motion'].setStiffnesses(names, stiffness)

	
	#self.move('LShoulderPitch', 1.5, 1, False)
	#self.move('LElbowRoll', 0.0, 1, False)
	#self.move('RElbowRoll', 0.0, 1, False)
	#self.move('RElbowYaw', 0.0, 1, False)
	#self.move('RShoulderPitch', 1.5, 1, False)
	#self.move('LShoulderRoll', 0.4, 1, False)
	#self.move('RShoulderRoll', -0.4, 1, False)
	time.sleep(1)

    def listen_shared_memory(self):

        speed = 0.3
        print "Listening shared memory..."
        while True:
            try:
                data = np.memmap(self.SHM_MOTORS_FILE, dtype=np.float32, mode='r')
                for i in xrange(0, 10):
                    #time.sleep(2)
                    if float(data[i]) != 0 :
	                    self.move(self.MOTORS_ORDER[i], float(data[i]), speed, True)
                    #print self.MOTORS_ORDER[i]
                    #print float(data[i])
            except IOError:
                print self.SHM_MOTORS_FILE + " -not found"
                time.sleep(0.05)

    def move(self, name, position, speed, normalize=True):
        if normalize:
            real_position = self._unnormalize_pos(name, position)
            #print "unormalize : " + str(real_position)
        else:
            real_position = position
        if self._check_motor(name, real_position):
            self.services['motion'].setAngles(name, real_position, speed)
#        else:
 #           print "Invalid move " + name + ": " + str(real_position)

if __name__ == '__main__':
    app = qi.Application(sys.argv)
    app.start()

    nao_caller = NaoCaller(app)
    nao_caller.listen_shared_memory()

    app.run() # block until session close or ctrl+c
