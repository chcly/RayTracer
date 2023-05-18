pipeline {
    agent any
    triggers {
        GenericTrigger(causeString: 'Relay Push Service',  token: 'FileTools.Build')
    }
    stages {
    
        stage('Update Submodules') {
            steps {
                sh script: 'git submodule update --init'
            }
        }

        stage('Build Project') {

            steps {
                sh script: 'cmake -DFileTools_BUILD_TEST=ON -DFileTools_AUTO_RUN_TEST=ON .'
                sh script: 'make'
            }
        }

        stage('Finalize') {
            steps {
                deleteDir()
            }
        }
    }
}