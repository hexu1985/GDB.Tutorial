/* MyClass.h */
/* Copyright (c) 2008 Thorsten Groetker, Ulrich Holtmann, Holger Keding, 
Markus Wloka. All rights reserved. Your use or disclosure of this source code 
is subject to the terms and conditions of the end user license agreement (EULA)
that is part of this software package. */

class MyClass {
public:
    MyClass() {
        int i; 
        for(i=0;i<10;i++) 
            a[i]= veryImportantStatic+i;
    }

    int a[10];
    static int veryImportantStatic;
};
