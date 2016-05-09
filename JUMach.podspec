Pod::Spec.new do |spec|

	spec.name = 'JUMach'
	spec.version = '0.1'
	spec.summary = 'Useful helper for cross-platform Mach development'
	spec.homepage = 'http://github.com/JustSid/JUMach'
	spec.authors = { 'Sidney Just' => 'justsid@widerwille.co' }
	spec.license = { :type => 'MIT' }
	spec.source = { :git => 'https://github.com/JustSid/JUMach.git', :tag => "v#{spec.version}" }
	spec.requires_arc = true
	spec.platforms = { :ios => '8.0', :osx => '10.10' }
	spec.source_files = [ 'Source/JU*.{c,h}' ]

end