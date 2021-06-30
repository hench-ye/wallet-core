Pod::Spec.new do |s|
  s.name         = 'TrustWalletCore'
  s.version      = '2.6.11'
  s.summary      = 'Trust Wallet core data structures and algorithms.'
  s.homepage     = 'https://github.com/hench-ye/wallet-core'
  s.license      = 'MIT'
  s.authors      = { 'Hench Ye' => 'hench.ye@topnetork.org' }
  s.module_name  = 'WalletCore'
  s.ios.deployment_target = '12.0'
  s.osx.deployment_target = '10.12'
  s.swift_version = '5.1'
  s.source = {
    :http => 'https://github.com/hench-ye/wallet-core/releases/download/2.6.11/TrustWalletCore-2.6.11.tar.xz'
  }
  s.default_subspec = 'Core'
  s.subspec 'Types' do |ss|
    ss.source_files = 
      'Sources/Types/*.swift',
      'Sources/Generated/Enums/*.swift',
      'Sources/Generated/Protobuf/*.swift'
    ss.dependency 'SwiftProtobuf'
  end
  s.subspec 'Core' do |ss|
    ss.vendored_frameworks = '*.xcframework'
    ss.exclude_files = 'Sources/Generated/WalletCore.h'
    ss.source_files =
      'include/**/*.h',
      'Sources/*.{swift,h,m,cpp}',
      'Sources/Extensions/*.swift',
      'Sources/Generated/*.{swift,h}'
    ss.public_header_files =
      'include/**/*.h',
      'Sources/*.h'
    ss.libraries = 'c++'
    ss.dependency 'TrustWalletCore/Types'
  end
end
