pipeline {
  agent any
  stages {
    stage('Builds') {
      parallel {
        stage('Windows Release|x86') {
          steps {
            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\MSBuild\\15.0\\Bin\\amd64\\MSBuild.exe" OpenHab2RulesGenerator.sln /p:configuration=Release /p:platform=x86 /t:rebuild'
          }
        }
        stage('Windows Debug|x86') {
          steps {
            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\MSBuild\\15.0\\Bin\\amd64\\MSBuild.exe" OpenHab2RulesGenerator.sln /p:configuration=Debug /p:platform=x86 /t:rebuild'
          }
        }
        stage('Windows Release|x64') {
          steps {
            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\MSBuild\\15.0\\Bin\\amd64\\MSBuild.exe" OpenHab2RulesGenerator.sln /p:configuration=Release /p:platform=x64 /t:rebuild'
          }
        }
        stage('Windows Debug|x64') {
          steps {
            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\MSBuild\\15.0\\Bin\\amd64\\MSBuild.exe" OpenHab2RulesGenerator.sln /p:configuration=Debug /p:platform=x64 /t:rebuild'
          }
        }
        stage('Linux') {
          agent {
            label 'linux_slave'
          }
          steps {
            sh 'make'
            archiveArtifacts(artifacts: 'bin/*/linux/OH2RulesGenerator-linux-*', onlyIfSuccessful: true)
          }
        }
      }
    }
    stage('Test') {
      steps {
        echo 'Testing done.'
      }
    }
    stage('Finish') {
      steps {
        archiveArtifacts(artifacts: 'bin/*/windows/*/OH2RulesGenerator-win10-*.exe', onlyIfSuccessful: true)
      }
    }
  }
}
