Start with Area 2 (IT and NOC) - 2 /29 subnets
Add 4 /30 point-to-point links after Area 2
Add 4 /30 backbone point-to-point links
Add Area 3 /29 subnet followed by 4 /30 point-to-point links
Add 6 /30 point-to-point links for Area 4
Add Area 5 /29 subnet followed by 4 /30 point-to-point links
Add 2 /29 subnets for Area 1 VLANs followed by 4 /30 point-to-point links

Starting with the base address **192.168.26.0/24** (256 IPs), subnets are allocated as follows:

### Area 2 - IT Department and NOC

#### 4.1. Area 2 - IT Department (6 hosts)
- **Subnet Size**: /29 (6 usable IPs)
- **Subnet Mask**: 255.255.255.248
- **Wildcard Mask**: 0.0.0.7
- **Assigned Subnet**: 192.168.26.0/29
- **IP Range**: 192.168.26.0 – 192.168.26.7
- **Usable IPs**: 192.168.26.1 – 192.168.26.6
- **Network Address**: 192.168.26.0
- **Broadcast Address**: 192.168.26.7
- **Next Available**: 192.168.26.8

#### 4.2. Area 2 - NOC (4 hosts)
- **Subnet Size**: /29 (6 usable IPs)
- **Subnet Mask**: 255.255.255.248
- **Wildcard Mask**: 0.0.0.7
- **Assigned Subnet**: 192.168.26.8/29
- **IP Range**: 192.168.26.8 – 192.168.26.15
- **Usable IPs**: 192.168.26.9 – 192.168.26.14
- **Network Address**: 192.168.26.8
- **Broadcast Address**: 192.168.26.15
- **Next Available**: 192.168.26.16

#### 4.3. Point-to-Point Link 1 (Area 2) Router 18 -19 se 3/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.16/30
- **IP Range**: 192.168.26.16 – 192.168.26.19
- **Usable IPs**: 192.168.26.17 – 192.168.26.18
- **Network Address**: 192.168.26.16
- **Broadcast Address**: 192.168.26.19
- **Next Available**: 192.168.26.20

#### 4.4. Point-to-Point Link 2 (Area 2) Router 18 - 17 se 2/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.20/30
- **IP Range**: 192.168.26.20 – 192.168.26.23
- **Usable IPs**: 192.168.26.21 – 192.168.26.22
- **Network Address**: 192.168.26.20
- **Broadcast Address**: 192.168.26.23
- **Next Available**: 192.168.26.24

#### 4.5. Point-to-Point Link 3 (Area 2) Router 17 - 19 (se 3/0)(se 2/0)
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.24/30
- **IP Range**: 192.168.26.24 – 192.168.26.27
- **Usable IPs**: 192.168.26.25 – 192.168.26.26
- **Network Address**: 192.168.26.24
- **Broadcast Address**: 192.168.26.27
- **Next Available**: 192.168.26.28

#### 4.6. Point-to-Point Link 4 (Area 2) Router 17 - 7 se 6/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.28/30
- **IP Range**: 192.168.26.28 – 192.168.26.31
- **Usable IPs**: 192.168.26.29 – 192.168.26.30
- **Network Address**: 192.168.26.28
- **Broadcast Address**: 192.168.26.31
- **Next Available**: 192.168.26.32

=====================================================================
### Backbone Point-to-Point Links

#### 4.7. Backbone Point-to-Point Link 1 Router 7-6 se 2/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.32/30
- **IP Range**: 192.168.26.32 – 192.168.26.35
- **Usable IPs**: 192.168.26.33 – 192.168.26.34
- **Network Address**: 192.168.26.32
- **Broadcast Address**: 192.168.26.35
- **Next Available**: 192.168.26.36

#### 4.8. Backbone Point-to-Point Link 2 Router 7-5 se 3/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.36/30
- **IP Range**: 192.168.26.36 – 192.168.26.39
- **Usable IPs**: 192.168.26.37 – 192.168.26.38
- **Network Address**: 192.168.26.36
- **Broadcast Address**: 192.168.26.39
- **Next Available**: 192.168.26.40

#### 4.9. Backbone Point-to-Point Link 3 Router 6-4 se 3/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.40/30
- **IP Range**: 192.168.26.40 – 192.168.26.43
- **Usable IPs**: 192.168.26.41 – 192.168.26.42
- **Network Address**: 192.168.26.40
- **Broadcast Address**: 192.168.26.43
- **Next Available**: 192.168.26.44

#### 4.10. Backbone Point-to-Point Link 4 Router 4-5 se 2/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.44/30
- **IP Range**: 192.168.26.44 – 192.168.26.47
- **Usable IPs**: 192.168.26.45 – 192.168.26.46
- **Network Address**: 192.168.26.44
- **Broadcast Address**: 192.168.26.47
- **Next Available**: 192.168.26.48

=====================================================================
### Area 3 - Postpaid Billing

#### 4.11. Area 3 - Postpaid Billing (3 hosts)
- **Subnet Size**: /29 (6 usable IPs)
- **Subnet Mask**: 255.255.255.248
- **Wildcard Mask**: 0.0.0.7
- **Assigned Subnet**: 192.168.26.48/29
- **IP Range**: 192.168.26.48 – 192.168.26.55
- **Usable IPs**: 192.168.26.49 – 192.168.26.54
- **Network Address**: 192.168.26.48
- **Broadcast Address**: 192.168.26.55
- **Next Available**: 192.168.26.56

#### 4.12. Point-to-Point Link 1 (Area 3) Router 12-13 se 3/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.56/30
- **IP Range**: 192.168.26.56 – 192.168.26.59
- **Usable IPs**: 192.168.26.57 – 192.168.26.58
- **Network Address**: 192.168.26.56
- **Broadcast Address**: 192.168.26.59
- **Next Available**: 192.168.26.60

#### 4.13. Point-to-Point Link 2 (Area 3) Router 13-11 se 2/0 se 3/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.60/30
- **IP Range**: 192.168.26.60 – 192.168.26.63
- **Usable IPs**: 192.168.26.61 – 192.168.26.62
- **Network Address**: 192.168.26.60
- **Broadcast Address**: 192.168.26.63
- **Next Available**: 192.168.26.64

#### 4.14. Point-to-Point Link 3 (Area 3) Router 11-12 se 2/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.64/30
- **IP Range**: 192.168.26.64 – 192.168.26.67
- **Usable IPs**: 192.168.26.65 – 192.168.26.66
- **Network Address**: 192.168.26.64
- **Broadcast Address**: 192.168.26.67
- **Next Available**: 192.168.26.68

#### 4.15. Point-to-Point Link 4 (Area 3) Router 11-6 se 6/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.68/30
- **IP Range**: 192.168.26.68 – 192.168.26.71
- **Usable IPs**: 192.168.26.69 – 192.168.26.70
- **Network Address**: 192.168.26.68
- **Broadcast Address**: 192.168.26.71
- **Next Available**: 192.168.26.72


=====================================================================
### Area 4 Point-to-Point Links

#### 4.16. Point-to-Point Link 1 (Area 4) fa 0/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.72/30
- **IP Range**: 192.168.26.72 – 192.168.26.75
- **Usable IPs**: 192.168.26.73 – 192.168.26.74
- **Network Address**: 192.168.26.72
- **Broadcast Address**: 192.168.26.75
- **Next Available**: 192.168.26.76

#### 4.17. Point-to-Point Link 2 (Area 4) Router 8-10 se 2/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.76/30
- **IP Range**: 192.168.26.76 – 192.168.26.79
- **Usable IPs**: 192.168.26.77 – 192.168.26.78
- **Network Address**: 192.168.26.76
- **Broadcast Address**: 192.168.26.79
- **Next Available**: 192.168.26.80

#### 4.18. Point-to-Point Link 3 (Area 4)Router 8-9 se 3/0 se 2/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.80/30
- **IP Range**: 192.168.26.80 – 192.168.26.83
- **Usable IPs**: 192.168.26.81 – 192.168.26.82
- **Network Address**: 192.168.26.80
- **Broadcast Address**: 192.168.26.83
- **Next Available**: 192.168.26.84

#### 4.19. Point-to-Point Link 4 (Area 4)Router 9-10 se 3/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.84/30
- **IP Range**: 192.168.26.84 – 192.168.26.87
- **Usable IPs**: 192.168.26.85 – 192.168.26.86
- **Network Address**: 192.168.26.84
- **Broadcast Address**: 192.168.26.87
- **Next Available**: 192.168.26.88


#### 4.20. Point-to-Point Link 5 (Area 4)Router 9-6 se 7/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.88/30
- **IP Range**: 192.168.26.88 – 192.168.26.91
- **Usable IPs**: 192.168.26.89 – 192.168.26.90
- **Network Address**: 192.168.26.88
- **Broadcast Address**: 192.168.26.91
- **Next Available**: 192.168.26.92

#### 4.21. Point-to-Point Link 6 (Area 4)Router 9-4 se 6/0 se 7/0 
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.92/30
- **IP Range**: 192.168.26.92 – 192.168.26.95
- **Usable IPs**: 192.168.26.93 – 192.168.26.94
- **Network Address**: 192.168.26.92
- **Broadcast Address**: 192.168.26.95
- **Next Available**: 192.168.26.96


=====================================================================
### Area 5 - OMD

#### 4.22. Area 5 - OMD (2 hosts)
- **Subnet Size**: /29 (6 usable IPs)
- **Subnet Mask**: 255.255.255.248
- **Wildcard Mask**: 0.0.0.7
- **Assigned Subnet**: 192.168.26.96/29
- **IP Range**: 192.168.26.96 – 192.168.26.103
- **Usable IPs**: 192.168.26.97 – 192.168.26.102
- **Network Address**: 192.168.26.96
- **Broadcast Address**: 192.168.26.103
- **Next Available**: 192.168.26.104

#### 4.23. Point-to-Point Link 1 (Area 5)Router 1-3 se 3/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.104/30
- **IP Range**: 192.168.26.104 – 192.168.26.107
- **Usable IPs**: 192.168.26.105 – 192.168.26.106
- **Network Address**: 192.168.26.104
- **Broadcast Address**: 192.168.26.107
- **Next Available**: 192.168.26.108

#### 4.24. Point-to-Point Link 2 (Area 5)Router 1-2 se 2/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.108/30
- **IP Range**: 192.168.26.108 – 192.168.26.111
- **Usable IPs**: 192.168.26.109 – 192.168.26.110
- **Network Address**: 192.168.26.108
- **Broadcast Address**: 192.168.26.111
- **Next Available**: 192.168.26.112

#### 4.25. Point-to-Point Link 3 (Area 5)Router 2-3 se 3/0 se 2/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.112/30
- **IP Range**: 192.168.26.112 – 192.168.26.115
- **Usable IPs**: 192.168.26.113 – 192.168.26.114
- **Network Address**: 192.168.26.112
- **Broadcast Address**: 192.168.26.115
- **Next Available**: 192.168.26.116

#### 4.26. Point-to-Point Link 4 (Area 5)Router 2-4 se 6/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.116/30
- **IP Range**: 192.168.26.116 – 192.168.26.119
- **Usable IPs**: 192.168.26.117 – 192.168.26.118
- **Network Address**: 192.168.26.116
- **Broadcast Address**: 192.168.26.119
- **Next Available**: 192.168.26.120

================================================================
### Area 1 - VLANs

#### 4.27. Area 1 - Postpaid VLAN (6 hosts) Switch 5 Vlan postpaid 10
- **Subnet Size**: /29 (6 usable IPs)
- **Subnet Mask**: 255.255.255.248
- **Wildcard Mask**: 0.0.0.7
- **Assigned Subnet**: 192.168.26.120/29
- **IP Range**: 192.168.26.120 – 192.168.26.127
- **Usable IPs**: 192.168.26.121 – 192.168.26.126
- **Network Address**: 192.168.26.120
- **Broadcast Address**: 192.168.26.127
- **Next Available**: 192.168.26.128

#### 4.28. Area 1 - Prepaid VLAN (5 hosts)Switch 5 Vlan prepaid 20
- **Subnet Size**: /29 (6 usable IPs)
- **Subnet Mask**: 255.255.255.248
- **Wildcard Mask**: 0.0.0.7
- **Assigned Subnet**: 192.168.26.128/29
- **IP Range**: 192.168.26.128 – 192.168.26.135
- **Usable IPs**: 192.168.26.129 – 192.168.26.134
- **Network Address**: 192.168.26.128
- **Broadcast Address**: 192.168.26.135
- **Next Available**: 192.168.26.136

#### 4.29. Point-to-Point Link 1 (Area 1)Router 14-15 se2/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.136/30
- **IP Range**: 192.168.26.136 – 192.168.26.139
- **Usable IPs**: 192.168.26.137 – 192.168.26.138
- **Network Address**: 192.168.26.136
- **Broadcast Address**: 192.168.26.139
- **Next Available**: 192.168.26.140

#### 4.30. Point-to-Point Link 2 (Area 1)Router 15-16 se 3/0 se 2/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.140/30
- **IP Range**: 192.168.26.140 – 192.168.26.143
- **Usable IPs**: 192.168.26.141 – 192.168.26.142
- **Network Address**: 192.168.26.140
- **Broadcast Address**: 192.168.26.143
- **Next Available**: 192.168.26.144

#### 4.31. Point-to-Point Link 3 (Area 1)Router 14-16 se 3/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.144/30
- **IP Range**: 192.168.26.144 – 192.168.26.147
- **Usable IPs**: 192.168.26.145 – 192.168.26.146
- **Network Address**: 192.168.26.144
- **Broadcast Address**: 192.168.26.147
- **Next Available**: 192.168.26.148

#### 4.32. Point-to-Point Link 4 (Area 1)Router 5-16 se 6/0
- **Subnet Size**: /30 (2 usable IPs)
- **Subnet Mask**: 255.255.255.252
- **Wildcard Mask**: 0.0.0.3
- **Assigned Subnet**: 192.168.26.148/30
- **IP Range**: 192.168.26.148 – 192.168.26.151
- **Usable IPs**: 192.168.26.149 – 192.168.26.150
- **Network Address**: 192.168.26.148
- **Broadcast Address**: 192.168.26.151
- **Next Available**: 192.168.26.152