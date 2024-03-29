/*
 * [The "BSD licence"]
 * Copyright (c) 2010 Ben Gruver (JesusFreke)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package org.jf.dexlib.Code.Analysis;

import org.jf.dexlib.*;
import org.jf.dexlib.Util.AccessFlags;
import org.jf.dexlib.Util.ExceptionWithContext;
import org.jf.dexlib.Util.SparseArray;

import java.io.File;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static org.jf.dexlib.ClassDataItem.EncodedField;
import static org.jf.dexlib.ClassDataItem.EncodedMethod;

public class ClassPath {
    private static ClassPath theClassPath = null;

    private final HashMap<String, ClassDef> classDefs;
    protected ClassDef javaLangObjectClassDef; //Ljava/lang/Object;

    //This is only used while initialing the class path. It is set to null after initialization has finished.
    private LinkedHashMap<String, TempClassInfo> tempClasses;


    private static final Pattern dalvikCacheOdexPattern = Pattern.compile("@([^@]+)@classes.dex$");


    public static interface ClassPathErrorHandler {
        void ClassPathError(String className, Exception ex);
    }

    /**
     * Initialize the class path using the dependencies from an odex file
     * @param classPathDirs The directories to search for boot class path files
     * @param extraBootClassPathEntries any extra entries that should be added after the entries that are read
     * from the odex file
     * @param dexFilePath The path of the dex file (used for error reporting purposes only)
     * @param dexFile The DexFile to load - it must represents an odex file
     * @param errorHandler a ClassPathErrorHandler object to receive and handle any errors that occur while loading
     * classes
     */
    public static void InitializeClassPathFromOdex(String[] classPathDirs, String[] extraBootClassPathEntries,
                                                   String dexFilePath, DexFile dexFile,
                                                   ClassPathErrorHandler errorHandler) {
        if (!dexFile.isOdex()) {
            throw new ExceptionWithContext("Cannot use InitialiazeClassPathFromOdex with a non-odex DexFile");
        }

        if (theClassPath != null) {
            throw new ExceptionWithContext("Cannot initialize ClassPath multiple times");
        }

        OdexDependencies odexDependencies = dexFile.getOdexDependencies();

        String[] bootClassPath = new String[odexDependencies.getDependencyCount()];
        for (int i=0; i<bootClassPath.length; i++) {
            String dependency = odexDependencies.getDependency(i);

            if (dependency.endsWith(".odex")) {
                int slashIndex = dependency.lastIndexOf("/");

                if (slashIndex != -1) {
                    dependency = dependency.substring(slashIndex+1);
                }
            } else if (dependency.endsWith("@classes.dex")) {
                Matcher m = dalvikCacheOdexPattern.matcher(dependency);

                if (!m.find()) {
                    throw new ExceptionWithContext(String.format("Cannot parse dependency value %s", dependency));
                }

                dependency = m.group(1);
            } else {
                throw new ExceptionWithContext(String.format("Cannot parse dependency value %s", dependency));
            }

            bootClassPath[i] = dependency;
        }

        theClassPath = new ClassPath();
        theClassPath.initClassPath(classPathDirs, bootClassPath, extraBootClassPathEntries, dexFilePath, dexFile,
                errorHandler);
    }

    /**
     * Initialize the class path using the given boot class path entries
     * @param classPathDirs The directories to search for boot class path files
     * @param bootClassPath A list of the boot class path entries to search for and load
     * @param dexFilePath The path of the dex file (used for error reporting purposes only)
     * @param dexFile the DexFile to load
     * @param errorHandler a ClassPathErrorHandler object to receive and handle any errors that occur while loading
     * classes
     */
    public static void InitializeClassPath(String[] classPathDirs, String[] bootClassPath,
                                           String[] extraBootClassPathEntries, String dexFilePath, DexFile dexFile,
                                           ClassPathErrorHandler errorHandler) {
        if (theClassPath != null) {
            throw new ExceptionWithContext("Cannot initialize ClassPath multiple times");
        }

        theClassPath = new ClassPath();
        theClassPath.initClassPath(classPathDirs, bootClassPath, extraBootClassPathEntries, dexFilePath, dexFile,
                errorHandler);
    }

    private ClassPath() {
        classDefs = new HashMap<String, ClassDef>();
    }

    private void initClassPath(String[] classPathDirs, String[] bootClassPath, String[] extraBootClassPathEntries,
                               String dexFilePath, DexFile dexFile, ClassPathErrorHandler errorHandler) {
        tempClasses = new LinkedHashMap<String, TempClassInfo>();

        if (bootClassPath != null) {
            for (String bootClassPathEntry: bootClassPath) {
                loadBootClassPath(classPathDirs, bootClassPathEntry);
            }
        }

        if (extraBootClassPathEntries != null) {
            for (String bootClassPathEntry: extraBootClassPathEntries) {
                loadBootClassPath(classPathDirs, bootClassPathEntry);
            }
        }

        if (dexFile != null) {
            loadDexFile(dexFilePath, dexFile);
        }


        for (String classType: tempClasses.keySet()) {
            ClassDef classDef = null;
            try {
                classDef = ClassPath.loadClassDef(classType);
                assert classDef != null;
            } catch (Exception ex) {
                if (errorHandler != null) {
                    errorHandler.ClassPathError(classType, ex);
                } else {
                    throw ExceptionWithContext.withContext(ex,
                            String.format("Error while loading ClassPath class %s", classType));
                }
            }

            if (classType.equals("Ljava/lang/Object;")) {
                this.javaLangObjectClassDef = classDef;
            }
        }

        for (String primitiveType: new String[]{"Z", "B", "S", "C", "I", "J", "F", "D"}) {
            ClassDef classDef = new PrimitiveClassDef(primitiveType);
            classDefs.put(primitiveType, classDef);
        }

        tempClasses = null;
    }

    private void loadBootClassPath(String[] classPathDirs, String bootClassPathEntry) {
        for (String classPathDir: classPathDirs) {
            File file = null;
            DexFile dexFile = null;

            int extIndex = bootClassPathEntry.lastIndexOf(".");

            String baseEntry;
            if (extIndex == -1) {
                baseEntry = bootClassPathEntry;
            } else {
                baseEntry = bootClassPathEntry.substring(0, extIndex);
            }

            for (String ext: new String[]{"", ".odex", ".jar", ".apk", ".zip"}) {
                if (ext.length() == 0) {
                    file = new File(classPathDir, bootClassPathEntry);
                } else {
                    file = new File(classPathDir, baseEntry + ext);
                }

                if (file.exists()) {
                    if (!file.canRead()) {
                        System.err.println(String.format("warning: cannot open %s for reading. Will continue " +
                                "looking.", file.getPath()));
                        continue;
                    }

                    try {
                        dexFile = new DexFile(file, false, true);
                    } catch (DexFile.NoClassesDexException ex) {
                        continue;
                    } catch (Exception ex) {
                        throw ExceptionWithContext.withContext(ex, "Error while reading boot class path entry \"" +
                        bootClassPathEntry + "\".");
                    }
                }
            }
            if (dexFile == null) {
                continue;
            }

            try {
                loadDexFile(file.getPath(), dexFile);
            } catch (Exception ex) {
                throw ExceptionWithContext.withContext(ex,
                        String.format("Error while loading boot classpath entry %s", bootClassPathEntry));
            }
            return;
        }
        throw new ExceptionWithContext(String.format("Cannot locate boot class path file %s", bootClassPathEntry));
    }

    private void loadDexFile(String dexFilePath, DexFile dexFile) {
        for (ClassDefItem classDefItem: dexFile.ClassDefsSection.getItems()) {
            try {
                //TODO: need to check if the class already exists. (and if so, what to do about it?)
                TempClassInfo tempClassInfo = new TempClassInfo(dexFilePath, classDefItem);

                tempClasses.put(tempClassInfo.classType, tempClassInfo);
            } catch (Exception ex) {
                throw ExceptionWithContext.withContext(ex, String.format("Error while loading class %s",
                        classDefItem.getClassType().getTypeDescriptor()));
            }
        }
    }

    private static class ClassNotFoundException extends ExceptionWithContext {
        public ClassNotFoundException(String message) {
            super(message);
        }
    }

    public static ClassDef getClassDef(String classType) {
        return getClassDef(classType, true);
    }

    /**
     * This method checks if the given class has been loaded yet. If it has, it returns the loaded ClassDef. If not,
     * then it looks up the TempClassItem for the given class and (possibly recursively) loads the ClassDef.
     * @param classType the class to load
     * @return the existing or newly loaded ClassDef object for the given class, or null if the class cannot be found
     */
    private static ClassDef loadClassDef(String classType) {
        ClassDef classDef = getClassDef(classType, false);

        if (classDef == null) {
            TempClassInfo classInfo = theClassPath.tempClasses.get(classType);
            if (classInfo == null) {
                return null;
            }

            try {
                classDef = new ClassDef(classInfo);
                theClassPath.classDefs.put(classDef.classType, classDef);
            } catch (Exception ex) {
                throw ExceptionWithContext.withContext(ex, String.format("Error while loading class %s from file %s",
                        classInfo.classType, classInfo.dexFilePath));
            }
        }
        return classDef;
    }

    public static ClassDef getClassDef(String classType, boolean createUnresolvedClassDef)  {
        ClassDef classDef = theClassPath.classDefs.get(classType);
        if (classDef == null) {
            //if it's an array class, try to create it
            if (classType.charAt(0) == '[') {
                return theClassPath.createArrayClassDef(classType);
            } else {
                if (createUnresolvedClassDef) {
                    //TODO: we should output a warning
                    return theClassPath.createUnresolvedClassDef(classType);
                } else {
                    return null;
                }
            }
        }
        return classDef;
    }

    public static ClassDef getClassDef(TypeIdItem classType) {
        return getClassDef(classType.getTypeDescriptor());
    }

    public static ClassDef getClassDef(TypeIdItem classType, boolean creatUnresolvedClassDef) {
        return getClassDef(classType.getTypeDescriptor(), creatUnresolvedClassDef);
    }

    //256 [ characters
    private static final String arrayPrefix = "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[" +
        "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[" +
        "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[";
    private static ClassDef getArrayClassDefByElementClassAndDimension(ClassDef classDef, int arrayDimension) {
        return getClassDef(arrayPrefix.substring(256 - arrayDimension) + classDef.classType);
    }

    private ClassDef createUnresolvedClassDef(String classType)  {
        assert classType.charAt(0) == 'L';

        UnresolvedClassDef unresolvedClassDef = new UnresolvedClassDef(classType);
        classDefs.put(classType, unresolvedClassDef);
        return unresolvedClassDef;
    }

    private ClassDef createArrayClassDef(String arrayClassName) {
        assert arrayClassName != null;
        assert arrayClassName.charAt(0) == '[';

        ArrayClassDef arrayClassDef = new ArrayClassDef(arrayClassName);
        if (arrayClassDef.elementClass == null) {
            return null;
        }

        classDefs.put(arrayClassName, arrayClassDef);
        return arrayClassDef;
    }

    public static ClassDef getCommonSuperclass(ClassDef class1, ClassDef class2) {
        if (class1 == class2) {
            return class1;
        }

        if (class1 == null) {
            return class2;
        }

        if (class2 == null) {
            return class1;
        }

        //TODO: do we want to handle primitive types here? I don't think so.. (if not, add assert)

        if (class2.isInterface) {
            if (class1.implementsInterface(class2)) {
                return class2;
            }
            return theClassPath.javaLangObjectClassDef;
        }

        if (class1.isInterface) {
            if (class2.implementsInterface(class1)) {
                return class1;
            }
            return theClassPath.javaLangObjectClassDef;
        }

        if (class1 instanceof ArrayClassDef && class2 instanceof ArrayClassDef) {
            return getCommonArraySuperclass((ArrayClassDef)class1, (ArrayClassDef)class2);
        }

        //we've got two non-array reference types. Find the class depth of each, and then move up the longer one
        //so that both classes are at the same class depth, and then move each class up until they match

        //we don't strictly need to keep track of the class depth separately, but it's probably slightly faster
        //to do so, rather than calling getClassDepth() many times
        int class1Depth = class1.getClassDepth();
        int class2Depth = class2.getClassDepth();

        while (class1Depth > class2Depth) {
            class1 = class1.superclass;
            class1Depth--;
        }

        while (class2Depth > class1Depth) {
            class2 = class2.superclass;
            class2Depth--;
        }

        while (class1Depth > 0) {
            if (class1 == class2) {
                return class1;
            }
            class1 = class1.superclass;
            class1Depth--;
            class2 = class2.superclass;
            class2Depth--;
        }

        return class1;
    }

    private static ClassDef getCommonArraySuperclass(ArrayClassDef class1, ArrayClassDef class2) {
        assert class1 != class2;

        //If one of the arrays is a primitive array, then the only option is to return java.lang.Object
        //TODO: might it be possible to merge something like int[] and short[] into int[]? (I don't think so..)
        if (class1.elementClass instanceof PrimitiveClassDef || class2.elementClass instanceof PrimitiveClassDef) {
            return theClassPath.javaLangObjectClassDef;
        }

        //if the two arrays have the same number of dimensions, then we should return an array class with the
        //same number of dimensions, for the common superclass of the 2 element classes
        if (class1.arrayDimensions == class2.arrayDimensions) {
            ClassDef commonElementClass = getCommonSuperclass(class1.elementClass, class2.elementClass);
            return getArrayClassDefByElementClassAndDimension(commonElementClass, class1.arrayDimensions);
        }

        //something like String[][][] and String[][] should be merged to Object[][]
        //this also holds when the element classes aren't the same (but are both reference types)
        int dimensions = Math.min(class1.arrayDimensions, class2.arrayDimensions);
        return getArrayClassDefByElementClassAndDimension(theClassPath.javaLangObjectClassDef, dimensions);
    }

    public static class ArrayClassDef extends ClassDef {
        private final ClassDef elementClass;
        private final int arrayDimensions;

        protected ArrayClassDef(String arrayClassType) {
            super(arrayClassType, ClassDef.ArrayClassDef);
            assert arrayClassType.charAt(0) == '[';

            int i=0;
            while (arrayClassType.charAt(i) == '[') i++;

            String elementClassType = arrayClassType.substring(i);

            if (i>256) {
                throw new ExceptionWithContext("Error while creating array class for element type " + elementClassType +
                        " with " + i + " dimensions. The maximum number of dimensions is 256");
            }

            try {
                elementClass = ClassPath.getClassDef(arrayClassType.substring(i));
            } catch (ClassNotFoundException ex) {
                throw ExceptionWithContext.withContext(ex, "Error while creating array class " + arrayClassType);
            }
            arrayDimensions = i;
        }

        /**
         * Returns the "base" element class of the array.
         *
         * For example, for a multi-dimensional array of strings ([[Ljava/lang/String;), this method would return
         * Ljava/lang/String;
         * @return the "base" element class of the array
         */
        public ClassDef getBaseElementClass() {
            return elementClass;
        }

        /**
         * Returns the "immediate" element class of the array.
         *
         * For example, for a multi-dimensional array of stings with 2 dimensions ([[Ljava/lang/String;), this method
         * would return [Ljava/lang/String;
         * @return the immediate element class of the array
         */
        public ClassDef getImmediateElementClass() {
            if (arrayDimensions == 1) {
                return elementClass;
            }
            return getArrayClassDefByElementClassAndDimension(elementClass, arrayDimensions - 1);
        }

        public int getArrayDimensions() {
            return arrayDimensions;
        }

        @Override
        public boolean extendsClass(ClassDef superclassDef) {
            if (!(superclassDef instanceof ArrayClassDef)) {
                if (superclassDef == ClassPath.theClassPath.javaLangObjectClassDef) {
                    return true;
                } else if (superclassDef.isInterface) {
                    return this.implementsInterface(superclassDef);
                }
                return false;
            }

            ArrayClassDef arraySuperclassDef = (ArrayClassDef)superclassDef;
            if (this.arrayDimensions == arraySuperclassDef.arrayDimensions) {
                ClassDef baseElementClass = arraySuperclassDef.getBaseElementClass();

                if (baseElementClass.isInterface) {
                    return true;
                }

                return baseElementClass.extendsClass(arraySuperclassDef.getBaseElementClass());
            } else if (this.arrayDimensions > arraySuperclassDef.arrayDimensions) {
                ClassDef baseElementClass = arraySuperclassDef.getBaseElementClass();
                if (baseElementClass.isInterface) {
                    return true;
                }

                if (baseElementClass == ClassPath.theClassPath.javaLangObjectClassDef) {
                    return true;
                }
                return false;
            }
            return false;
        }
    }

    public static class PrimitiveClassDef extends ClassDef {
        protected PrimitiveClassDef(String primitiveClassType) {
            super(primitiveClassType, ClassDef.PrimitiveClassDef);
            assert primitiveClassType.charAt(0) != 'L' && primitiveClassType.charAt(0) != '[';
        }
    }

    public static class UnresolvedClassDef extends ClassDef {
        protected UnresolvedClassDef(String unresolvedClassDef) {
            super(unresolvedClassDef, ClassDef.UnresolvedClassDef);
            assert unresolvedClassDef.charAt(0) == 'L';
        }

        protected ValidationException unresolvedValidationException() {
            return new ValidationException(String.format("class %s cannot be resolved.", this.getClassType()));
        }

        public ClassDef getSuperclass() {
            throw unresolvedValidationException();
        }

        public int getClassDepth() {
            throw unresolvedValidationException();
        }

        public boolean isInterface() {
            throw unresolvedValidationException();
        }

         public boolean extendsClass(ClassDef superclassDef) {
            if (superclassDef != theClassPath.javaLangObjectClassDef && superclassDef != this) {
                throw unresolvedValidationException();
            }
            return true;
        }

        public boolean implementsInterface(ClassDef interfaceDef) {
            throw unresolvedValidationException();
        }

        public boolean hasVirtualMethod(String method) {
            if (!super.hasVirtualMethod(method)) {
                throw unresolvedValidationException();
            }
            return true;
        }
    }

    public static class ClassDef implements Comparable<ClassDef> {
        private final String classType;
        private final ClassDef superclass;
        /**
         * This is a list of all of the interfaces that a class implements, either directly or indirectly. It includes
         * all interfaces implemented by the superclass, and all super-interfaces of any implemented interface. The
         * intention is to make it easier to determine whether the class implements a given interface or not.
         */
        private final TreeSet<ClassDef> implementedInterfaces;

        private final boolean isInterface;

        private final int classDepth;

        private final String[] vtable;
        private final HashMap<String, Integer> virtualMethodLookup;

        private final SparseArray<String> instanceFields;
        private final HashMap<String, Integer> instanceFieldLookup;

        public final static int ArrayClassDef = 0;
        public final static int PrimitiveClassDef = 1;
        public final static int UnresolvedClassDef = 2;


        /**
         * The following fields are used only during the initial loading of classes, and are set to null afterwards
         * TODO: free these
         */

        //This is only the virtual methods that this class declares itself.
        private String[] virtualMethods;
        //this is a list of all the interfaces that the class implements directory, or any super interfaces of those
        //interfaces. It is generated in such a way that it is ordered in the same way as dalvik's ClassObject.iftable,
        private LinkedHashMap<String, ClassDef> interfaceTable;

        /**
         * This constructor is used for the ArrayClassDef, PrimitiveClassDef and UnresolvedClassDef subclasses
         * @param classType the class type
         * @param classFlavor one of ArrayClassDef, PrimitiveClassDef or UnresolvedClassDef
         */
        protected ClassDef(String classType, int classFlavor) {
            if (classFlavor == ArrayClassDef) {
                assert classType.charAt(0) == '[';
                this.classType = classType;
                this.superclass = ClassPath.theClassPath.javaLangObjectClassDef;
                implementedInterfaces = new TreeSet<ClassDef>();
                implementedInterfaces.add(ClassPath.getClassDef("Ljava/lang/Cloneable;"));
                implementedInterfaces.add(ClassPath.getClassDef("Ljava/io/Serializable;"));
                isInterface = false;

                vtable = superclass.vtable;
                virtualMethodLookup = superclass.virtualMethodLookup;

                instanceFields = superclass.instanceFields;
                instanceFieldLookup = superclass.instanceFieldLookup;
                classDepth = 1; //1 off from java.lang.Object

                virtualMethods = null;
                interfaceTable = null;
            } else if (classFlavor == PrimitiveClassDef) {
                //primitive type
                assert classType.charAt(0) != '[' && classType.charAt(0) != 'L';

                this.classType = classType;
                this.superclass = null;
                implementedInterfaces = null;
                isInterface = false;
                vtable = null;
                virtualMethodLookup = null;
                instanceFields = null;
                instanceFieldLookup = null;
                classDepth = 0; //TODO: maybe use -1 to indicate not applicable?

                virtualMethods = null;
                interfaceTable = null;
            } else /*if (classFlavor == UnresolvedClassDef)*/ {
                assert classType.charAt(0) == 'L';
                this.classType = classType;
                this.superclass = ClassPath.theClassPath.javaLangObjectClassDef;
                implementedInterfaces = new TreeSet<ClassDef>();
                isInterface = false;

                vtable = superclass.vtable;
                virtualMethodLookup = superclass.virtualMethodLookup;

                instanceFields = superclass.instanceFields;
                instanceFieldLookup = superclass.instanceFieldLookup;
                classDepth = 1; //1 off from java.lang.Object

                virtualMethods = null;
                interfaceTable = null;
            }
        }

        protected ClassDef(TempClassInfo classInfo)  {
            classType = classInfo.classType;
            isInterface = classInfo.isInterface;

            superclass = loadSuperclass(classInfo);
            if (superclass == null) {
                classDepth = 0;
            } else {
                classDepth = superclass.classDepth + 1;
            }

            implementedInterfaces = loadAllImplementedInterfaces(classInfo);

            //TODO: we can probably get away with only creating the interface table for interface types
            interfaceTable = loadInterfaceTable(classInfo);
            virtualMethods = classInfo.virtualMethods;
            vtable = loadVtable(classInfo);
            virtualMethodLookup = new HashMap<String, Integer>((int)Math.ceil(vtable.length / .7f), .75f);
            for (int i=0; i<vtable.length; i++) {
                virtualMethodLookup.put(vtable[i], i);
            }

            instanceFields = loadFields(classInfo);
            instanceFieldLookup = new HashMap<String, Integer>((int)Math.ceil(instanceFields.size() / .7f), .75f);
            for (int i=0; i<instanceFields.size(); i++) {
                instanceFieldLookup.put(instanceFields.get(i), i);
            }
        }

        public String getClassType() {
            return classType;
        }

        public ClassDef getSuperclass() {
            return superclass;
        }

        public int getClassDepth() {
            return classDepth;
        }

        public boolean isInterface() {
            return this.isInterface;
        }

        public boolean extendsClass(ClassDef superclassDef) {
            if (superclassDef == null) {
                return false;
            }

            if (this == superclassDef) {
                return true;
            }

            if (superclassDef instanceof UnresolvedClassDef) {
                throw ((UnresolvedClassDef)superclassDef).unresolvedValidationException();
            }

            int superclassDepth = superclassDef.classDepth;
            ClassDef ancestor = this;
            while (ancestor.classDepth > superclassDepth) {
                ancestor = ancestor.getSuperclass();
            }

            return ancestor == superclassDef;
        }

        /**
         * Returns true if this class implements the given interface. This searches the interfaces that this class
         * directly implements, any interface implemented by this class's superclasses, and any super-interface of
         * any of these interfaces.
         * @param interfaceDef the interface
         * @return true if this class implements the given interface
         */
        public boolean implementsInterface(ClassDef interfaceDef) {
            assert !(interfaceDef instanceof UnresolvedClassDef);
            return implementedInterfaces.contains(interfaceDef);
        }

        public boolean hasVirtualMethod(String method) {
            return virtualMethodLookup.containsKey(method);
        }

        public String getInstanceField(int fieldOffset) {
            return this.instanceFields.get(fieldOffset, null);
        }

        public String getVirtualMethod(int vtableIndex) {
            if (vtableIndex < 0 || vtableIndex >= vtable.length) {
                return null;
            }
            return this.vtable[vtableIndex];
        }

        private void swap(byte[] fieldTypes, String[] fields, int position1, int position2) {
            byte tempType = fieldTypes[position1];
            fieldTypes[position1] = fieldTypes[position2];
            fieldTypes[position2] = tempType;

            String tempField = fields[position1];
            fields[position1] = fields[position2];
            fields[position2] = tempField;
        }

        private ClassDef loadSuperclass(TempClassInfo classInfo) {
            if (classInfo.classType.equals("Ljava/lang/Object;")) {
                if (classInfo.superclassType != null) {
                    throw new ExceptionWithContext("Invalid superclass " +
                            classInfo.superclassType + " for Ljava/lang/Object;. " +
                            "The Object class cannot have a superclass");
                }
                return null;
            } else {
                String superclassType = classInfo.superclassType;
                if (superclassType == null) {
                    throw new ExceptionWithContext(classInfo.classType + " has no superclass");
                }

                ClassDef superclass = ClassPath.loadClassDef(superclassType);
                if (superclass == null) {
                    throw new ClassNotFoundException(String.format("Could not find superclass %s", superclassType));
                }

                if (!isInterface && superclass.isInterface) {
                    throw new ValidationException("Class " + classType + " has the interface " + superclass.classType +
                            " as its superclass");
                }
                if (isInterface && !superclass.isInterface && superclass !=
                        ClassPath.theClassPath.javaLangObjectClassDef) {
                    throw new ValidationException("Interface " + classType + " has the non-interface class " +
                            superclass.classType + " as its superclass");
                }

                return superclass;
            }
        }

        private TreeSet<ClassDef> loadAllImplementedInterfaces(TempClassInfo classInfo) {
            assert classType != null;
            assert classType.equals("Ljava/lang/Object;") || superclass != null;
            assert classInfo != null;

            TreeSet<ClassDef> implementedInterfaceSet = new TreeSet<ClassDef>();

            if (superclass != null) {
                for (ClassDef interfaceDef: superclass.implementedInterfaces) {
                    implementedInterfaceSet.add(interfaceDef);
                }
            }


            if (classInfo.interfaces != null) {
                for (String interfaceType: classInfo.interfaces) {
                    ClassDef interfaceDef = ClassPath.loadClassDef(interfaceType);
                    if (interfaceDef == null) {
                        throw new ClassNotFoundException(String.format("Could not find interface %s", interfaceType));
                    }
                    assert interfaceDef.isInterface();
                    implementedInterfaceSet.add(interfaceDef);

                    interfaceDef = interfaceDef.getSuperclass();
                    while (!interfaceDef.getClassType().equals("Ljava/lang/Object;")) {
                        assert interfaceDef.isInterface();
                        implementedInterfaceSet.add(interfaceDef);
                        interfaceDef = interfaceDef.getSuperclass();
                    }
                }
            }

            return implementedInterfaceSet;
        }

        private LinkedHashMap<String, ClassDef> loadInterfaceTable(TempClassInfo classInfo) {
            if (classInfo.interfaces == null) {
                return null;
            }

            LinkedHashMap<String, ClassDef> interfaceTable = new LinkedHashMap<String, ClassDef>();

            for (String interfaceType: classInfo.interfaces) {
                if (!interfaceTable.containsKey(interfaceType)) {
                    ClassDef interfaceDef = ClassPath.loadClassDef(interfaceType);
                    if (interfaceDef == null) {
                        throw new ClassNotFoundException(String.format("Could not find interface %s", interfaceType));
                    }
                    interfaceTable.put(interfaceType, interfaceDef);

                    if (interfaceDef.interfaceTable != null) {
                        for (ClassDef superInterface: interfaceDef.interfaceTable.values()) {
                            if (!interfaceTable.containsKey(superInterface.classType)) {
                                interfaceTable.put(superInterface.classType, superInterface);
                            }
                        }
                    }
                }
            }

            return interfaceTable;
        }

        private String[] loadVtable(TempClassInfo classInfo) {
            //TODO: it might be useful to keep track of which class's implementation is used for each virtual method. In other words, associate the implementing class type with each vtable entry
            List<String> virtualMethodList = new LinkedList<String>();
            //use a temp hash table, so that we can construct the final lookup with an appropriate
            //capacity, based on the number of virtual methods
            HashMap<String, Integer> tempVirtualMethodLookup = new HashMap<String, Integer>();

            //copy the virtual methods from the superclass
            int methodIndex = 0;
            if (superclass != null) {
                for (String method: superclass.vtable) {
                    virtualMethodList.add(method);
                    tempVirtualMethodLookup.put(method, methodIndex++);
                }

                assert superclass.instanceFields != null;
            }


            //iterate over the virtual methods in the current class, and only add them when we don't already have the
            //method (i.e. if it was implemented by the superclass)
            if (!this.isInterface) {
                if (classInfo.virtualMethods != null) {
                    for (String virtualMethod: classInfo.virtualMethods) {
                        if (tempVirtualMethodLookup.get(virtualMethod) == null) {
                            virtualMethodList.add(virtualMethod);
                            tempVirtualMethodLookup.put(virtualMethod, methodIndex++);
                        }
                    }
                }

                if (interfaceTable != null) {
                    for (ClassDef interfaceDef: interfaceTable.values()) {
                        if (interfaceDef.virtualMethods == null) {
                            continue;
                        }

                        for (String virtualMethod: interfaceDef.virtualMethods) {
                            if (tempVirtualMethodLookup.get(virtualMethod) == null) {
                                virtualMethodList.add(virtualMethod);
                                tempVirtualMethodLookup.put(virtualMethod, methodIndex++);
                            }
                        }
                    }
                }
            }

            String[] vtable = new String[virtualMethodList.size()];
            for (int i=0; i<virtualMethodList.size(); i++) {
                vtable[i] = virtualMethodList.get(i);
            }

            return vtable;
        }

        private int getNextFieldOffset() {
            if (instanceFields == null || instanceFields.size() == 0) {
                return 8;
            }

            int lastItemIndex = instanceFields.size()-1;
            int fieldOffset = instanceFields.keyAt(lastItemIndex);
            String lastField = instanceFields.valueAt(lastItemIndex);

            int fieldTypeIndex = lastField.indexOf(":") + 1;

            switch (lastField.charAt(fieldTypeIndex)) {
                case 'J':
                case 'D':
                    return fieldOffset + 8;
                default:
                    return fieldOffset + 4;
            }
        }

        private SparseArray<String> loadFields(TempClassInfo classInfo) {
            //This is a bit of an "involved" operation. We need to follow the same algorithm that dalvik uses to
            //arrange fields, so that we end up with the same field offsets (which is needed for deodexing).
            //See mydroid/dalvik/vm/oo/Class.c - computeFieldOffsets()

            final byte REFERENCE = 0;
            final byte WIDE = 1;
            final byte OTHER = 2;

            String[] fields = null;
            //the "type" for each field in fields. 0=reference,1=wide,2=other
            byte[] fieldTypes = null;

            if (classInfo.instanceFields != null) {
                fields = new String[classInfo.instanceFields.length];
                fieldTypes = new byte[fields.length];

                for (int i=0; i<fields.length; i++) {
                    String[] fieldInfo = classInfo.instanceFields[i];

                    String fieldName = fieldInfo[0];
                    String fieldType = fieldInfo[1];

                    String field = String.format("%s:%s", fieldName, fieldType);
                    fieldTypes[i] = getFieldType(fieldType);
                    fields[i] = field;
                }
            }

            if (fields == null) {
                fields = new String[0];
                fieldTypes = new byte[0];
            }

            //The first operation is to move all of the reference fields to the front. To do this, find the first
            //non-reference field, then find the last reference field, swap them and repeat
            int back = fields.length - 1;
            int front;
            for (front = 0; front<fields.length; front++) {
                if (fieldTypes[front] != REFERENCE) {
                    while (back > front) {
                        if (fieldTypes[back] == REFERENCE) {
                            swap(fieldTypes, fields, front, back--);
                            break;
                        }
                        back--;
                    }
                }

                if (fieldTypes[front] != REFERENCE) {
                    break;
                }
            }


            int startFieldOffset = 8;
            if (this.superclass != null) {
                startFieldOffset = this.superclass.getNextFieldOffset();
            }

            int fieldIndexMod;
            if ((startFieldOffset % 8) == 0) {
                fieldIndexMod = 0;
            } else {
                fieldIndexMod = 1;
            }

            //next, we need to group all the wide fields after the reference fields. But the wide fields have to be
            //8-byte aligned. If we're on an odd field index, we need to insert a 32-bit field. If the next field
            //is already a 32-bit field, use that. Otherwise, find the first 32-bit field from the end and swap it in.
            //If there are no 32-bit fields, do nothing for now. We'll add padding when calculating the field offsets
            if (front < fields.length && (front % 2) != fieldIndexMod) {
                if (fieldTypes[front] == WIDE) {
                    //we need to swap in a 32-bit field, so the wide fields will be correctly aligned
                    back = fields.length - 1;
                    while (back > front) {
                        if (fieldTypes[back] == OTHER) {
                            swap(fieldTypes, fields, front++, back);
                            break;
                        }
                        back--;
                    }
                } else {
                    //there's already a 32-bit field here that we can use
                    front++;
                }
            }

            //do the swap thing for wide fields
            back = fields.length - 1;
            for (; front<fields.length; front++) {
                if (fieldTypes[front] != WIDE) {
                    while (back > front) {
                        if (fieldTypes[back] == WIDE) {
                            swap(fieldTypes, fields, front, back--);
                            break;
                        }
                        back--;
                    }
                }

                if (fieldTypes[front] != WIDE) {
                    break;
                }
            }

            int superFieldCount = 0;
            if (superclass != null) {
                superFieldCount = superclass.instanceFields.size();
            }

            //now the fields are in the correct order. Add them to the SparseArray and lookup, and calculate the offsets
            int totalFieldCount = superFieldCount + fields.length;
            SparseArray<String> instanceFields = new SparseArray<String>(totalFieldCount);

            int fieldOffset;

            if (superclass != null && superFieldCount > 0) {
                for (int i=0; i<superFieldCount; i++) {
                    instanceFields.append(superclass.instanceFields.keyAt(i), superclass.instanceFields.valueAt(i));
                }

                fieldOffset = instanceFields.keyAt(superFieldCount-1);

                String lastSuperField = superclass.instanceFields.valueAt(superFieldCount-1);
                assert lastSuperField.indexOf(':') >= 0;
                assert lastSuperField.indexOf(':') < lastSuperField.length()-1; //the ':' shouldn't be the last char
                char fieldType = lastSuperField.charAt(lastSuperField.indexOf(':') + 1);
                if (fieldType == 'J' || fieldType == 'D') {
                    fieldOffset += 8;
                } else {
                    fieldOffset += 4;
                }
            } else {
                //the field values start at 8 bytes into the DataObject dalvik structure
                fieldOffset = 8;
            }

            boolean gotDouble = false;
            for (int i=0; i<fields.length; i++) {
                String field = fields[i];

                //add padding to align the wide fields, if needed
                if (fieldTypes[i] == WIDE && !gotDouble) {
                    if (!gotDouble) {
                        if (fieldOffset % 8 != 0) {
                            assert fieldOffset % 8 == 4;
                            fieldOffset += 4;
                        }
                        gotDouble = true;
                    }
                }

                instanceFields.append(fieldOffset, field);
                if (fieldTypes[i] == WIDE) {
                    fieldOffset += 8;
                } else {
                    fieldOffset += 4;
                }
            }
            return instanceFields;
        }

        private byte getFieldType(String fieldType) {
            switch (fieldType.charAt(0)) {
                case '[':
                case 'L':
                    return 0; //REFERENCE
                case 'J':
                case 'D':
                    return 1; //WIDE
                default:
                    return 2; //OTHER
            }
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof ClassDef)) return false;

            ClassDef classDef = (ClassDef) o;

            return classType.equals(classDef.classType);
        }

        @Override
        public int hashCode() {
            return classType.hashCode();
        }

        public int compareTo(ClassDef classDef) {
            return classType.compareTo(classDef.classType);
        }
    }

    /**
     * In some cases, classes can reference another class (i.e. a superclass or an interface) that is in a *later*
     * boot class path entry. So we load all classes from all boot class path entries before starting to process them
     */
    private static class TempClassInfo {
        public final String dexFilePath;
        public final String classType;
        public final boolean isInterface;
        public final String superclassType;
        public final String[] interfaces;
        public final String[] virtualMethods;
        public final String[][] instanceFields;

        public TempClassInfo(String dexFilePath, ClassDefItem classDefItem) {
            this.dexFilePath = dexFilePath;

            classType = classDefItem.getClassType().getTypeDescriptor();

            isInterface = (classDefItem.getAccessFlags() & AccessFlags.INTERFACE.getValue()) != 0;

            TypeIdItem superclassType = classDefItem.getSuperclass();
            if (superclassType == null) {
                this.superclassType = null;
            } else {
                this.superclassType = superclassType.getTypeDescriptor();
            }

            interfaces = loadInterfaces(classDefItem);

            ClassDataItem classDataItem = classDefItem.getClassData();
            if (classDataItem != null) {
                virtualMethods = loadVirtualMethods(classDataItem);
                instanceFields = loadInstanceFields(classDataItem);
            } else {
                virtualMethods = null;
                instanceFields = null;
            }
        }

        private String[] loadInterfaces(ClassDefItem classDefItem) {
            TypeListItem typeList = classDefItem.getInterfaces();
            if (typeList != null) {
                List<TypeIdItem> types = typeList.getTypes();
                if (types != null && types.size() > 0) {
                    String[] interfaces = new String[types.size()];
                    for (int i=0; i<interfaces.length; i++) {
                        interfaces[i] = types.get(i).getTypeDescriptor();
                    }
                    return interfaces;
                }
            }
            return null;
        }

        private String[] loadVirtualMethods(ClassDataItem classDataItem) {
            EncodedMethod[] encodedMethods = classDataItem.getVirtualMethods();
            if (encodedMethods != null && encodedMethods.length > 0) {
                String[] virtualMethods = new String[encodedMethods.length];
                for (int i=0; i<encodedMethods.length; i++) {
                    virtualMethods[i] = encodedMethods[i].method.getVirtualMethodString();
                }
                return virtualMethods;
            }
            return null;
        }

        private String[][] loadInstanceFields(ClassDataItem classDataItem) {
            EncodedField[] encodedFields = classDataItem.getInstanceFields();
            if (encodedFields != null && encodedFields.length > 0) {
                String[][] instanceFields = new String[encodedFields.length][2];
                for (int i=0; i<encodedFields.length; i++) {
                    EncodedField encodedField = encodedFields[i];
                    instanceFields[i][0] = encodedField.field.getFieldName().getStringValue();
                    instanceFields[i][1] = encodedField.field.getFieldType().getTypeDescriptor();
                }
                return instanceFields;
            }
            return null;
        }
    }


    public static void validateAgainstDeodexerant(String host, int port, int skipClasses) {
        Deodexerant deodexerant = new Deodexerant(host, port);

        int count = 0;

        try {
            String[] inlineMethods = deodexerant.getInlineMethods();
            (new DeodexUtil(null)).checkInlineMethods(inlineMethods);
        } catch (Exception ex) {
            throw ExceptionWithContext.withContext(ex, "Error while checking inline methods");
        }

        try {
            for (ClassDef classDef: theClassPath.classDefs.values()) {
                if (count < skipClasses) {
                    count++;
                    continue;
                }

                if ((count%1000)==0) {
                    System.out.println(count);
                }
                if (classDef instanceof UnresolvedClassDef || classDef instanceof ArrayClassDef ||
                        classDef instanceof PrimitiveClassDef) {
                    continue;
                }

                String[] vtable = deodexerant.getVirtualMethods(classDef.classType);

                if (vtable.length != classDef.vtable.length) {
                    throw new ValidationException(String.format("virtual table size mismatch for class %s",
                            classDef.classType));
                }

                for (int i=0; i<classDef.vtable.length; i++) {
                    if (!classDef.vtable[i].equals(vtable[i])) {
                        throw new ValidationException(String.format("virtual method mismatch for class %s at index %d",
                                classDef.classType, i));
                    }
                }

                String[] fields = deodexerant.getInstanceFields(classDef.classType);

                if (fields.length != classDef.instanceFields.size()) {
                    throw new ValidationException(String.format("field count mismatch for class %s",
                            classDef.classType));
                }

                for (int i=0; i<classDef.instanceFields.size(); i++) {
                    String[] fieldValues = fields[i].split(" ");
                    if (fieldValues.length != 2) {
                        throw new ValidationException("Could not parse field");
                    }

                    int fieldOffset = Integer.parseInt(fieldValues[0]);

                    String field = classDef.instanceFields.get(fieldOffset);
                    if (field == null) {
                        throw new ValidationException("Could not find a field at the given offset");
                    }

                    if (!field.equals(fieldValues[1])) {
                        throw new ValidationException(String.format("field offset mismatch for class %s at index %d",
                                classDef.classType, i));
                    }
                }

                count++;
            }
        } catch (Exception ex) {
            throw ExceptionWithContext.withContext(ex, String.format("Error while checking class #%d", count));
        }
    }
}
