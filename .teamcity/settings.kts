import jetbrains.buildServer.configs.kotlin.*
import jetbrains.buildServer.configs.kotlin.buildSteps.script
import jetbrains.buildServer.configs.kotlin.triggers.vcs

/*
The settings script is an entry point for defining a TeamCity
project hierarchy. The script should contain a single call to the
project() function with a Project instance or an init function as
an argument.

VcsRoots, BuildTypes, Templates, and subprojects can be
registered inside the project using the vcsRoot(), buildType(),
template(), and subProject() methods respectively.

To debug settings scripts in command-line, run the

    mvnDebug org.jetbrains.teamcity:teamcity-configs-maven-plugin:generate

command and attach your debugger to the port 8000.

To debug in IntelliJ Idea, open the 'Maven Projects' tool window (View
-> Tool Windows -> Maven Projects), find the generate task node
(Plugins -> teamcity-configs -> teamcity-configs:generate), the
'Debug' option is available in the context menu for the task.
*/

version = "2024.03"

project {
    description = "ACF - Application Component Framework"

    vcsRoot(GitVcsRoot)

    buildType(CMakeBuildLinux)
    buildType(CMakeBuildWindows)
    buildType(QMakeBuildLinux)
    buildType(QMakeBuildWindows)
}

object GitVcsRoot : VcsRoot({
    name = "ACF Git Repository"
    type = "jetbrains.git"
    param("url", "https://github.com/ImagingTools/Acf.git")
    param("branch", "refs/heads/main")
    param("authMethod", "ANONYMOUS")
})

object CMakeBuildLinux : BuildType({
    name = "CMake Build (Linux)"
    
    vcs {
        root(GitVcsRoot)
    }
    
    steps {
        script {
            name = "Install Dependencies"
            scriptContent = """
                sudo apt-get update
                sudo apt-get install -y ninja-build qt6-base-dev qt6-tools-dev qt6-tools-dev-tools
            """.trimIndent()
        }
        
        script {
            name = "Set Environment Variables"
            scriptContent = """
                export ACFDIR=%teamcity.build.checkoutDir%
                export ACFCONFIGDIR=%teamcity.build.checkoutDir%/Config
                echo "##teamcity[setParameter name='env.ACFDIR' value='%teamcity.build.checkoutDir%']"
                echo "##teamcity[setParameter name='env.ACFCONFIGDIR' value='%teamcity.build.checkoutDir%/Config']"
            """.trimIndent()
        }
        
        script {
            name = "Generate Version Files"
            workingDir = "Build/Git"
            scriptContent = "./GenerateVersion.sh"
        }
        
        script {
            name = "Configure CMake"
            scriptContent = """
                cmake -S Build/CMake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
            """.trimIndent()
        }
        
        script {
            name = "Build"
            scriptContent = "cmake --build build --config Release"
        }
        
        script {
            name = "Run Tests"
            scriptContent = "ctest --test-dir build --output-on-failure -C Release"
        }
    }
    
    triggers {
        vcs {
            branchFilter = "+:*"
        }
    }
    
    requirements {
        contains("teamcity.agent.jvm.os.name", "Linux")
    }
})

object CMakeBuildWindows : BuildType({
    name = "CMake Build (Windows)"
    
    vcs {
        root(GitVcsRoot)
    }
    
    steps {
        script {
            name = "Install Ninja"
            scriptContent = "choco install ninja -y"
        }
        
        script {
            name = "Set Environment Variables"
            scriptContent = """
                set ACFDIR=%teamcity.build.checkoutDir%
                set ACFCONFIGDIR=%teamcity.build.checkoutDir%\Config
                echo ##teamcity[setParameter name='env.ACFDIR' value='%teamcity.build.checkoutDir%']
                echo ##teamcity[setParameter name='env.ACFCONFIGDIR' value='%teamcity.build.checkoutDir%\Config']
            """.trimIndent()
        }
        
        script {
            name = "Generate Version Files"
            workingDir = "Build\\Git"
            scriptContent = "call GenerateVersion.bat"
        }
        
        script {
            name = "Configure CMake"
            scriptContent = """
                cmake -S Build\CMake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
            """.trimIndent()
        }
        
        script {
            name = "Build"
            scriptContent = "cmake --build build --config Release"
        }
        
        script {
            name = "Run Tests"
            scriptContent = "ctest --test-dir build --output-on-failure -C Release"
        }
    }
    
    triggers {
        vcs {
            branchFilter = "+:*"
        }
    }
    
    requirements {
        contains("teamcity.agent.jvm.os.name", "Windows")
    }
})

object QMakeBuildLinux : BuildType({
    name = "QMake Build (Linux)"
    
    vcs {
        root(GitVcsRoot)
    }
    
    steps {
        script {
            name = "Install Dependencies"
            scriptContent = """
                sudo apt-get update
                sudo apt-get install -y qt6-base-dev qt6-tools-dev qt6-tools-dev-tools
            """.trimIndent()
        }
        
        script {
            name = "Set Environment Variables"
            scriptContent = """
                export ACFDIR=%teamcity.build.checkoutDir%
                export ACFCONFIGDIR=%teamcity.build.checkoutDir%/Config
                echo "##teamcity[setParameter name='env.ACFDIR' value='%teamcity.build.checkoutDir%']"
                echo "##teamcity[setParameter name='env.ACFCONFIGDIR' value='%teamcity.build.checkoutDir%/Config']"
            """.trimIndent()
        }
        
        script {
            name = "Generate Version Files"
            workingDir = "Build/Git"
            scriptContent = "./GenerateVersion.sh"
        }
        
        script {
            name = "Find and Build with QMake"
            scriptContent = """
                PRO_FILE=${'$'}(find . -name "*.pro" | sort | head -n 1)
                if [ -z "${'$'}PRO_FILE" ]; then
                    echo "Error: No .pro file found"
                    exit 1
                fi
                echo "Building: ${'$'}PRO_FILE"
                qmake6 ${'$'}PRO_FILE
                make -j${'$'}(nproc)
            """.trimIndent()
        }
    }
    
    triggers {
        vcs {
            branchFilter = "+:*"
        }
    }
    
    requirements {
        contains("teamcity.agent.jvm.os.name", "Linux")
    }
})

object QMakeBuildWindows : BuildType({
    name = "QMake Build (Windows)"
    
    vcs {
        root(GitVcsRoot)
    }
    
    steps {
        script {
            name = "Set Environment Variables"
            scriptContent = """
                set ACFDIR=%teamcity.build.checkoutDir%
                set ACFCONFIGDIR=%teamcity.build.checkoutDir%\Config
                echo ##teamcity[setParameter name='env.ACFDIR' value='%teamcity.build.checkoutDir%']
                echo ##teamcity[setParameter name='env.ACFCONFIGDIR' value='%teamcity.build.checkoutDir%\Config']
            """.trimIndent()
        }
        
        script {
            name = "Generate Version Files"
            workingDir = "Build\\Git"
            scriptContent = "call GenerateVersion.bat"
        }
        
        script {
            name = "Find and Build with QMake"
            scriptContent = """
                @echo off
                for /f "delims=" %%i in ('dir /b /s *.pro') do (
                    set PRO_FILE=%%i
                    goto :found
                )
                echo Error: No .pro file found
                exit /b 1
                :found
                echo Building: %PRO_FILE%
                qmake %PRO_FILE%
                if errorlevel 1 exit /b 1
                nmake
            """.trimIndent()
        }
    }
    
    triggers {
        vcs {
            branchFilter = "+:*"
        }
    }
    
    requirements {
        contains("teamcity.agent.jvm.os.name", "Windows")
    }
})
