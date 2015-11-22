#!/bin/sh

# cp /vol/bitbucket/ise3/robots/lib/cartographerstuff.jar .
# jar xvf cartographerstuff.jar



java -Djava.rmi.server.codebase=http://www.doc.ic.ac.uk/~pl198/project/ -Djava.security.policy=/homes/pl198/src/y3/project/uk/ac/ic/doc/ise3/policy.txt uk.ac.ic.doc.ise3.cart3.run $1 test $2
