# OsoyooSmartCarMain

This is a rebuild of the original code. We will be slowly converting this version to our version until we can replicate the error we are seeing in our version. 

Had a few hiccups with the latest updates but we managed to rebuild the code. The bug that prevented the obstacle status from updating inside the collisionAvoidance has been removed. 
Now we can turn around when the path forward is more than 80% blocked

Next we should fine tune the turning options so the collisionAvoidance is more precise.

That worked pretty well. We split the existing binary signature into turn, half turn, hard turn options. 

Next we should update the binary signature so that instead of 5 directions it looks in more directions. 
Will need to explore how many degrees we can look in and respond to. 