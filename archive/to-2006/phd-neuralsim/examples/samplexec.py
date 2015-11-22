
import Brainlib

#
#                  Log            ("string")
#                  Alert          ("string")
# bool             Ask            ("string?")
#                  Clear          ()
#
# Map              Nodes          ()
# Map              Edges          ()
#
# string           Name           (SerializableIter)
# string           Type           (SerializableIter)
#                  Recall         (SerializableIter)
#                  Learn          (SerializableIter)
#                  Reset          (SerializableIter)
#
# StringList       List           ("name")
#
# SerializableIter Map.__iter__
#
# bool             StringList.has ("string")
#


#
# This sample recall will execute all synaptic
# layers which are output_modifiers first, then 
# all neural layers, and then the remaining
# synaptic layers.
#
def recall():
    Brainlib.Clear()
    Brainlib.Log("recalling...")

    om = Brainlib.List("output_modifiers")

    nodes = Brainlib.Nodes()
    edges = Brainlib.Edges()   

    for i in edges:
        if ( om.has(Brainlib.Type(i)) ):
            Brainlib.Recall(i)
            
    for i in nodes:
        Brainlib.Recall(i)

    for i in edges:
        if ( not om.has(Brainlib.Type(i)) ):
            Brainlib.Recall(i)
        
    Brainlib.Log("...if this message shows, there are probably no errors.")
    return
    

def learn():
    print "Learning"
    return

